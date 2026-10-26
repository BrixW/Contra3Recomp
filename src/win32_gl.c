/*
 * Contra III: The Alien Wars (USA) - OpenGL 3.3 Core Presentation Backend
 *
 * Uploads the 256x224 ARGB8888 S-PPU frame buffer to a single streaming
 * GL_RGBA8 texture each frame and draws one full-screen triangle through a
 * GLSL 3.30 fragment shader offering sharp-bilinear, nearest and CRT filters.
 * Only opengl32.dll is linked; every post-1.1 entry point is loaded through
 * wglGetProcAddress so machines without a capable driver fall back to GDI.
 */

#include "win32_gl.h"

#include <GL/gl.h>
#include <stdio.h>
#include <string.h>

/* --- GL / WGL constants not present in the GL 1.1 system header --- */
#define C3_GL_BGRA                        0x80E1
#define C3_GL_UNSIGNED_INT_8_8_8_8_REV    0x8367
#define C3_GL_CLAMP_TO_EDGE               0x812F
#define C3_GL_FRAGMENT_SHADER             0x8B30
#define C3_GL_VERTEX_SHADER               0x8B31
#define C3_GL_COMPILE_STATUS              0x8B81
#define C3_GL_LINK_STATUS                 0x8B82
#define C3_GL_TEXTURE0                    0x84C0

#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001

typedef char GLchar_c3;

typedef HGLRC  (WINAPI *PFN_wglCreateContextAttribsARB)(HDC, HGLRC, const int *);
typedef BOOL   (WINAPI *PFN_wglSwapIntervalEXT)(int);
typedef GLuint (APIENTRY *PFN_glCreateShader)(GLenum);
typedef void   (APIENTRY *PFN_glShaderSource)(GLuint, GLsizei, const GLchar_c3 *const *, const GLint *);
typedef void   (APIENTRY *PFN_glCompileShader)(GLuint);
typedef void   (APIENTRY *PFN_glGetShaderiv)(GLuint, GLenum, GLint *);
typedef void   (APIENTRY *PFN_glGetShaderInfoLog)(GLuint, GLsizei, GLsizei *, GLchar_c3 *);
typedef void   (APIENTRY *PFN_glDeleteShader)(GLuint);
typedef GLuint (APIENTRY *PFN_glCreateProgram)(void);
typedef void   (APIENTRY *PFN_glAttachShader)(GLuint, GLuint);
typedef void   (APIENTRY *PFN_glLinkProgram)(GLuint);
typedef void   (APIENTRY *PFN_glGetProgramiv)(GLuint, GLenum, GLint *);
typedef void   (APIENTRY *PFN_glGetProgramInfoLog)(GLuint, GLsizei, GLsizei *, GLchar_c3 *);
typedef void   (APIENTRY *PFN_glUseProgram)(GLuint);
typedef void   (APIENTRY *PFN_glDeleteProgram)(GLuint);
typedef GLint  (APIENTRY *PFN_glGetUniformLocation)(GLuint, const GLchar_c3 *);
typedef void   (APIENTRY *PFN_glUniform1i)(GLint, GLint);
typedef void   (APIENTRY *PFN_glUniform2f)(GLint, GLfloat, GLfloat);
typedef void   (APIENTRY *PFN_glGenVertexArrays)(GLsizei, GLuint *);
typedef void   (APIENTRY *PFN_glBindVertexArray)(GLuint);
typedef void   (APIENTRY *PFN_glDeleteVertexArrays)(GLsizei, const GLuint *);
typedef void   (APIENTRY *PFN_glActiveTexture)(GLenum);

static struct {
    HGLRC  rc;
    GLuint program;
    GLuint vao;
    GLuint tex;
    GLint  u_tex, u_src, u_dst, u_mode;
    int    tex_w, tex_h;
    char   renderer[160];
    uint32_t *capture;

    PFN_wglSwapIntervalEXT   SwapIntervalEXT;
    PFN_glCreateShader       CreateShader;
    PFN_glShaderSource       ShaderSource;
    PFN_glCompileShader      CompileShader;
    PFN_glGetShaderiv        GetShaderiv;
    PFN_glGetShaderInfoLog   GetShaderInfoLog;
    PFN_glDeleteShader       DeleteShader;
    PFN_glCreateProgram      CreateProgram;
    PFN_glAttachShader       AttachShader;
    PFN_glLinkProgram        LinkProgram;
    PFN_glGetProgramiv       GetProgramiv;
    PFN_glGetProgramInfoLog  GetProgramInfoLog;
    PFN_glUseProgram         UseProgram;
    PFN_glDeleteProgram      DeleteProgram;
    PFN_glGetUniformLocation GetUniformLocation;
    PFN_glUniform1i          Uniform1i;
    PFN_glUniform2f          Uniform2f;
    PFN_glGenVertexArrays    GenVertexArrays;
    PFN_glBindVertexArray    BindVertexArray;
    PFN_glDeleteVertexArrays DeleteVertexArrays;
    PFN_glActiveTexture      ActiveTexture;
} g_gl;

static const char *const k_vertex_src =
    "#version 330 core\n"
    "out vec2 v_uv;\n"
    "void main() {\n"
    "    vec2 pos = vec2(float((gl_VertexID << 1) & 2), float(gl_VertexID & 2));\n"
    "    v_uv = vec2(pos.x, 1.0 - pos.y);\n"
    "    gl_Position = vec4(pos * 2.0 - 1.0, 0.0, 1.0);\n"
    "}\n";

static const char *const k_fragment_src =
    "#version 330 core\n"
    "in vec2 v_uv;\n"
    "out vec4 o_color;\n"
    "uniform sampler2D u_tex;\n"
    "uniform vec2 u_src;\n"
    "uniform vec2 u_dst;\n"
    "uniform int  u_mode;\n"
    "\n"
    "vec3 fetch_nearest(vec2 uv) {\n"
    "    ivec2 t = ivec2(clamp(floor(uv * u_src), vec2(0.0), u_src - 1.0));\n"
    "    return texelFetch(u_tex, t, 0).rgb;\n"
    "}\n"
    "\n"
    "/* Sharp-bilinear: integer prescale emulated analytically, bilinear only across texel seams */\n"
    "vec3 fetch_sharp(vec2 uv) {\n"
    "    vec2 texel = uv * u_src;\n"
    "    vec2 scale = max(floor(u_dst / u_src), vec2(1.0));\n"
    "    vec2 base  = floor(texel);\n"
    "    vec2 cd    = fract(texel) - 0.5;\n"
    "    vec2 rr    = 0.5 - 0.5 / scale;\n"
    "    vec2 f     = (cd - clamp(cd, -rr, rr)) * scale + 0.5;\n"
    "    return texture(u_tex, (base + f) / u_src).rgb;\n"
    "}\n"
    "\n"
    "vec3 to_linear(vec3 c) { return pow(c, vec3(2.4)); }\n"
    "\n"
    "float beam(float d, float lum) {\n"
    "    float k = mix(9.0, 4.5, lum);\n"
    "    return exp(-d * d * k);\n"
    "}\n"
    "\n"
    "vec3 crt(vec2 uv) {\n"
    "    vec2 c = uv * 2.0 - 1.0;\n"
    "    c *= vec2(1.0 + c.y * c.y * 0.031, 1.0 + c.x * c.x * 0.041);\n"
    "    vec2 w = c * 0.5 + 0.5;\n"
    "    if (any(lessThan(w, vec2(0.0))) || any(greaterThan(w, vec2(1.0)))) return vec3(0.0);\n"
    "    float ty  = w.y * u_src.y;\n"
    "    float row = floor(ty);\n"
    "    float dy  = fract(ty) - 0.5;\n"
    "    float nb  = (dy < 0.0) ? -1.0 : 1.0;\n"
    "    vec3 c0 = to_linear(fetch_sharp(vec2(w.x, (row + 0.5) / u_src.y)));\n"
    "    vec3 c1 = to_linear(fetch_sharp(vec2(w.x, clamp(row + nb + 0.5, 0.5, u_src.y - 0.5) / u_src.y)));\n"
    "    float l0 = dot(c0, vec3(0.299, 0.587, 0.114));\n"
    "    float l1 = dot(c1, vec3(0.299, 0.587, 0.114));\n"
    "    vec3 col = c0 * beam(dy * 2.0, l0) + c1 * beam((1.0 - abs(dy)) * 2.0, l1);\n"
    "    if (u_dst.x / u_src.x >= 2.5) {\n"
    "        int m = int(mod(gl_FragCoord.x, 3.0));\n"
    "        vec3 mask = vec3(0.82);\n"
    "        mask[m] = 1.18;\n"
    "        col *= mask;\n"
    "    }\n"
    "    vec2 v = w * (1.0 - w.yx);\n"
    "    col *= clamp(pow(v.x * v.y * 15.0, 0.18), 0.0, 1.0);\n"
    "    return pow(col * 1.35, vec3(1.0 / 2.2));\n"
    "}\n"
    "\n"
    "void main() {\n"
    "    vec3 c;\n"
    "    if (u_mode == 1) c = fetch_nearest(v_uv);\n"
    "    else if (u_mode == 2 && u_dst.y >= u_src.y * 2.0) c = crt(v_uv);\n"
    "    else c = fetch_sharp(v_uv);\n"
    "    o_color = vec4(c, 1.0);\n"
    "}\n";

static bool load_proc(void *dst, const char *name) {
    PROC p = wglGetProcAddress(name);
    /* Some ICDs return small sentinel values instead of NULL for unsupported names */
    if (p == NULL || p == (PROC)1 || p == (PROC)2 || p == (PROC)3 || p == (PROC)-1) {
        return false;
    }
    memcpy(dst, &p, sizeof(p));
    return true;
}

static GLuint compile_shader(GLenum type, const char *src) {
    GLuint sh = g_gl.CreateShader(type);
    g_gl.ShaderSource(sh, 1, &src, NULL);
    g_gl.CompileShader(sh);
    GLint ok = 0;
    g_gl.GetShaderiv(sh, C3_GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024];
        g_gl.GetShaderInfoLog(sh, (GLsizei)sizeof(log), NULL, log);
        fprintf(stderr, "[GL] shader compile failed:\n%s\n", log);
        g_gl.DeleteShader(sh);
        return 0u;
    }
    return sh;
}

static void destroy_context(void) {
    if (g_gl.rc != NULL) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(g_gl.rc);
    }
    memset(&g_gl, 0, sizeof(g_gl));
}

bool win32_gl_init(HWND hwnd, HDC hdc, bool vsync) {
    (void)hwnd;
    memset(&g_gl, 0, sizeof(g_gl));

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize      = sizeof(pfd);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int fmt = ChoosePixelFormat(hdc, &pfd);
    if (fmt == 0 || !SetPixelFormat(hdc, fmt, &pfd)) {
        fprintf(stderr, "[GL] no OpenGL pixel format; using GDI\n");
        return false;
    }

    /* Bootstrap context -> request a 3.3 Core context, else keep a >=3.3 compatibility context */
    HGLRC boot = wglCreateContext(hdc);
    if (boot == NULL || !wglMakeCurrent(hdc, boot)) {
        if (boot != NULL) {
            wglDeleteContext(boot);
        }
        fprintf(stderr, "[GL] wglCreateContext failed; using GDI\n");
        return false;
    }
    PFN_wglCreateContextAttribsARB create_attribs = NULL;
    load_proc(&create_attribs, "wglCreateContextAttribsARB");
    if (create_attribs != NULL) {
        const int attribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
            WGL_CONTEXT_MINOR_VERSION_ARB, 3,
            WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };
        HGLRC core = create_attribs(hdc, NULL, attribs);
        if (core != NULL) {
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(boot);
            boot = core;
            wglMakeCurrent(hdc, boot);
        }
    }
    g_gl.rc = boot;

    const char *ver = (const char *)glGetString(GL_VERSION);
    const char *ren = (const char *)glGetString(GL_RENDERER);
    int major = (ver != NULL) ? (ver[0] - '0') : 0;
    int minor = (ver != NULL && ver[1] == '.') ? (ver[2] - '0') : 0;
    if (major < 3 || (major == 3 && minor < 3)) {
        fprintf(stderr, "[GL] OpenGL %s < 3.3; using GDI\n", ver != NULL ? ver : "?");
        destroy_context();
        return false;
    }
    snprintf(g_gl.renderer, sizeof(g_gl.renderer), "OpenGL %.24s | %.100s", ver, ren != NULL ? ren : "?");

    bool ok = true;
    ok &= load_proc(&g_gl.CreateShader,       "glCreateShader");
    ok &= load_proc(&g_gl.ShaderSource,       "glShaderSource");
    ok &= load_proc(&g_gl.CompileShader,      "glCompileShader");
    ok &= load_proc(&g_gl.GetShaderiv,        "glGetShaderiv");
    ok &= load_proc(&g_gl.GetShaderInfoLog,   "glGetShaderInfoLog");
    ok &= load_proc(&g_gl.DeleteShader,       "glDeleteShader");
    ok &= load_proc(&g_gl.CreateProgram,      "glCreateProgram");
    ok &= load_proc(&g_gl.AttachShader,       "glAttachShader");
    ok &= load_proc(&g_gl.LinkProgram,        "glLinkProgram");
    ok &= load_proc(&g_gl.GetProgramiv,       "glGetProgramiv");
    ok &= load_proc(&g_gl.GetProgramInfoLog,  "glGetProgramInfoLog");
    ok &= load_proc(&g_gl.UseProgram,         "glUseProgram");
    ok &= load_proc(&g_gl.DeleteProgram,      "glDeleteProgram");
    ok &= load_proc(&g_gl.GetUniformLocation, "glGetUniformLocation");
    ok &= load_proc(&g_gl.Uniform1i,          "glUniform1i");
    ok &= load_proc(&g_gl.Uniform2f,          "glUniform2f");
    ok &= load_proc(&g_gl.GenVertexArrays,    "glGenVertexArrays");
    ok &= load_proc(&g_gl.BindVertexArray,    "glBindVertexArray");
    ok &= load_proc(&g_gl.DeleteVertexArrays, "glDeleteVertexArrays");
    ok &= load_proc(&g_gl.ActiveTexture,      "glActiveTexture");
    load_proc(&g_gl.SwapIntervalEXT, "wglSwapIntervalEXT");
    if (!ok) {
        fprintf(stderr, "[GL] missing GL 3.3 entry points; using GDI\n");
        destroy_context();
        return false;
    }

    GLuint vs = compile_shader(C3_GL_VERTEX_SHADER, k_vertex_src);
    GLuint fs = compile_shader(C3_GL_FRAGMENT_SHADER, k_fragment_src);
    if (vs == 0u || fs == 0u) {
        destroy_context();
        return false;
    }
    g_gl.program = g_gl.CreateProgram();
    g_gl.AttachShader(g_gl.program, vs);
    g_gl.AttachShader(g_gl.program, fs);
    g_gl.LinkProgram(g_gl.program);
    g_gl.DeleteShader(vs);
    g_gl.DeleteShader(fs);
    GLint linked = 0;
    g_gl.GetProgramiv(g_gl.program, C3_GL_LINK_STATUS, &linked);
    if (!linked) {
        char log[1024];
        g_gl.GetProgramInfoLog(g_gl.program, (GLsizei)sizeof(log), NULL, log);
        fprintf(stderr, "[GL] program link failed:\n%s\n", log);
        destroy_context();
        return false;
    }
    g_gl.u_tex  = g_gl.GetUniformLocation(g_gl.program, "u_tex");
    g_gl.u_src  = g_gl.GetUniformLocation(g_gl.program, "u_src");
    g_gl.u_dst  = g_gl.GetUniformLocation(g_gl.program, "u_dst");
    g_gl.u_mode = g_gl.GetUniformLocation(g_gl.program, "u_mode");

    g_gl.GenVertexArrays(1, &g_gl.vao);
    glGenTextures(1, &g_gl.tex);
    glBindTexture(GL_TEXTURE_2D, g_gl.tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, C3_GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, C3_GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    win32_gl_set_vsync(vsync);
    while (glGetError() != GL_NO_ERROR) {
    }
    return true;
}

void win32_gl_set_vsync(bool on) {
    if (g_gl.SwapIntervalEXT != NULL) {
        g_gl.SwapIntervalEXT(on ? 1 : 0);
    }
}

bool win32_gl_vsync_supported(void) {
    return g_gl.SwapIntervalEXT != NULL;
}

void win32_gl_capture_next(uint32_t *dst) {
    g_gl.capture = dst;
}

const char *win32_gl_renderer(void) {
    return g_gl.renderer;
}

const char *win32_gl_filter_name(Win32GlFilter filter) {
    switch (filter) {
        case WIN32_GL_FILTER_SHARP:   return "Sharp-Bilinear";
        case WIN32_GL_FILTER_NEAREST: return "Nearest";
        case WIN32_GL_FILTER_CRT:     return "CRT";
        default:                      return "?";
    }
}

void win32_gl_present(HDC hdc, const uint32_t *argb, int src_w, int src_h,
                      int win_w, int win_h, bool integer_scale, Win32GlFilter filter) {
    if (g_gl.rc == NULL || win_w <= 0 || win_h <= 0) {
        return;
    }

    /* Square-pixel 8:7 placement (matches GDI path), optionally snapped to an integer multiple */
    int dst_w = win_w;
    int dst_h = (win_w * src_h) / src_w;
    if (dst_h > win_h) {
        dst_h = win_h;
        dst_w = (win_h * src_w) / src_h;
    }
    if (integer_scale) {
        int sx = win_w / src_w;
        int sy = win_h / src_h;
        int s = (sx < sy) ? sx : sy;
        if (s >= 1) {
            dst_w = src_w * s;
            dst_h = src_h * s;
        }
    }
    int dst_x = (win_w - dst_w) / 2;
    int dst_y = (win_h - dst_h) / 2;

    glViewport(0, 0, win_w, win_h);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    g_gl.ActiveTexture(C3_GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_gl.tex);
    if (g_gl.tex_w != src_w || g_gl.tex_h != src_h) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, src_w, src_h, 0,
                     C3_GL_BGRA, C3_GL_UNSIGNED_INT_8_8_8_8_REV, argb);
        g_gl.tex_w = src_w;
        g_gl.tex_h = src_h;
    } else {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, src_w, src_h,
                        C3_GL_BGRA, C3_GL_UNSIGNED_INT_8_8_8_8_REV, argb);
    }

    glViewport(dst_x, win_h - dst_y - dst_h, dst_w, dst_h);
    g_gl.UseProgram(g_gl.program);
    g_gl.Uniform1i(g_gl.u_tex, 0);
    g_gl.Uniform2f(g_gl.u_src, (GLfloat)src_w, (GLfloat)src_h);
    g_gl.Uniform2f(g_gl.u_dst, (GLfloat)dst_w, (GLfloat)dst_h);
    g_gl.Uniform1i(g_gl.u_mode, (GLint)filter);
    g_gl.BindVertexArray(g_gl.vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    if (g_gl.capture != NULL) {
        /* Back-buffer readback (bottom-up) -> top-down ARGB for --gl-selftest */
        glPixelStorei(GL_PACK_ALIGNMENT, 4);
        glReadPixels(0, 0, win_w, win_h, C3_GL_BGRA, C3_GL_UNSIGNED_INT_8_8_8_8_REV, g_gl.capture);
        for (int y = 0; y < win_h / 2; ++y) {
            uint32_t *a = g_gl.capture + (size_t)y * (size_t)win_w;
            uint32_t *b = g_gl.capture + (size_t)(win_h - 1 - y) * (size_t)win_w;
            for (int x = 0; x < win_w; ++x) {
                uint32_t t = a[x];
                a[x] = b[x];
                b[x] = t;
            }
        }
        g_gl.capture = NULL;
    }

    SwapBuffers(hdc);
}

void win32_gl_shutdown(HWND hwnd, HDC hdc) {
    (void)hwnd;
    (void)hdc;
    if (g_gl.rc == NULL) {
        return;
    }
    if (g_gl.tex != 0u) {
        glDeleteTextures(1, &g_gl.tex);
    }
    if (g_gl.vao != 0u) {
        g_gl.DeleteVertexArrays(1, &g_gl.vao);
    }
    if (g_gl.program != 0u) {
        g_gl.DeleteProgram(g_gl.program);
    }
    destroy_context();
}
