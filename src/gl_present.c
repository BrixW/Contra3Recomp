/*
 * Contra III: The Alien Wars (USA) - portable OpenGL 3.3 Core presenter
 */

#include "gl_present.h"

#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
#define C3_APIENTRY __stdcall
#else
#define C3_APIENTRY
#endif

typedef unsigned int   GLenum_;
typedef unsigned int   GLuint_;
typedef int            GLint_;
typedef int            GLsizei_;
typedef float          GLfloat_;
typedef unsigned int   GLbitfield_;
typedef unsigned char  GLubyte_;
typedef char           GLchar_;

#define GL_COLOR_BUFFER_BIT_          0x00004000
#define GL_TRIANGLES_                 0x0004
#define GL_TEXTURE_2D_                0x0DE1
#define GL_TEXTURE_MAG_FILTER_        0x2800
#define GL_TEXTURE_MIN_FILTER_        0x2801
#define GL_TEXTURE_WRAP_S_            0x2802
#define GL_TEXTURE_WRAP_T_            0x2803
#define GL_LINEAR_                    0x2601
#define GL_UNPACK_ALIGNMENT_          0x0CF5
#define GL_PACK_ALIGNMENT_            0x0D05
#define GL_RGBA8_                     0x8058
#define GL_VERSION_                   0x1F02
#define GL_RENDERER_                  0x1F01
#define GL_NO_ERROR_                  0
#define GL_BGRA_                      0x80E1
#define GL_UNSIGNED_INT_8_8_8_8_REV_  0x8367
#define GL_CLAMP_TO_EDGE_             0x812F
#define GL_FRAGMENT_SHADER_           0x8B30
#define GL_VERTEX_SHADER_             0x8B31
#define GL_COMPILE_STATUS_            0x8B81
#define GL_LINK_STATUS_               0x8B82
#define GL_TEXTURE0_                  0x84C0

static struct {
    bool   ready;
    GLuint_ program, vao, tex;
    GLint_  u_tex, u_src, u_dst, u_mode;
    int     tex_w, tex_h;
    uint32_t *capture;
    char    renderer[160];

    const GLubyte_ *(C3_APIENTRY *GetString)(GLenum_);
    GLenum_ (C3_APIENTRY *GetError)(void);
    void (C3_APIENTRY *Viewport)(GLint_, GLint_, GLsizei_, GLsizei_);
    void (C3_APIENTRY *ClearColor)(GLfloat_, GLfloat_, GLfloat_, GLfloat_);
    void (C3_APIENTRY *Clear)(GLbitfield_);
    void (C3_APIENTRY *GenTextures)(GLsizei_, GLuint_ *);
    void (C3_APIENTRY *DeleteTextures)(GLsizei_, const GLuint_ *);
    void (C3_APIENTRY *BindTexture)(GLenum_, GLuint_);
    void (C3_APIENTRY *TexParameteri)(GLenum_, GLenum_, GLint_);
    void (C3_APIENTRY *TexImage2D)(GLenum_, GLint_, GLint_, GLsizei_, GLsizei_, GLint_, GLenum_, GLenum_, const void *);
    void (C3_APIENTRY *TexSubImage2D)(GLenum_, GLint_, GLint_, GLint_, GLsizei_, GLsizei_, GLenum_, GLenum_, const void *);
    void (C3_APIENTRY *PixelStorei)(GLenum_, GLint_);
    void (C3_APIENTRY *ReadPixels)(GLint_, GLint_, GLsizei_, GLsizei_, GLenum_, GLenum_, void *);
    void (C3_APIENTRY *DrawArrays)(GLenum_, GLint_, GLsizei_);
    GLuint_ (C3_APIENTRY *CreateShader)(GLenum_);
    void (C3_APIENTRY *ShaderSource)(GLuint_, GLsizei_, const GLchar_ *const *, const GLint_ *);
    void (C3_APIENTRY *CompileShader)(GLuint_);
    void (C3_APIENTRY *GetShaderiv)(GLuint_, GLenum_, GLint_ *);
    void (C3_APIENTRY *GetShaderInfoLog)(GLuint_, GLsizei_, GLsizei_ *, GLchar_ *);
    void (C3_APIENTRY *DeleteShader)(GLuint_);
    GLuint_ (C3_APIENTRY *CreateProgram)(void);
    void (C3_APIENTRY *AttachShader)(GLuint_, GLuint_);
    void (C3_APIENTRY *LinkProgram)(GLuint_);
    void (C3_APIENTRY *GetProgramiv)(GLuint_, GLenum_, GLint_ *);
    void (C3_APIENTRY *GetProgramInfoLog)(GLuint_, GLsizei_, GLsizei_ *, GLchar_ *);
    void (C3_APIENTRY *UseProgram)(GLuint_);
    void (C3_APIENTRY *DeleteProgram)(GLuint_);
    GLint_ (C3_APIENTRY *GetUniformLocation)(GLuint_, const GLchar_ *);
    void (C3_APIENTRY *Uniform1i)(GLint_, GLint_);
    void (C3_APIENTRY *Uniform2f)(GLint_, GLfloat_, GLfloat_);
    void (C3_APIENTRY *GenVertexArrays)(GLsizei_, GLuint_ *);
    void (C3_APIENTRY *BindVertexArray)(GLuint_);
    void (C3_APIENTRY *DeleteVertexArrays)(GLsizei_, const GLuint_ *);
    void (C3_APIENTRY *ActiveTexture)(GLenum_);
} g;

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

static bool load(C3GlGetProc get_proc, void *dst, const char *name) {
    void *p = get_proc(name);
    if (p == NULL) {
        return false;
    }
    memcpy(dst, &p, sizeof(p));
    return true;
}

static GLuint_ compile_shader(GLenum_ type, const char *src) {
    GLuint_ sh = g.CreateShader(type);
    g.ShaderSource(sh, 1, &src, NULL);
    g.CompileShader(sh);
    GLint_ ok = 0;
    g.GetShaderiv(sh, GL_COMPILE_STATUS_, &ok);
    if (!ok) {
        char log[1024];
        g.GetShaderInfoLog(sh, (GLsizei_)sizeof(log), NULL, log);
        fprintf(stderr, "[GL] shader compile failed:\n%s\n", log);
        g.DeleteShader(sh);
        return 0u;
    }
    return sh;
}

bool c3gl_init(C3GlGetProc get_proc) {
    memset(&g, 0, sizeof(g));
    bool ok = true;
#define L(field, name) ok &= load(get_proc, &g.field, name)
    L(GetString, "glGetString");             L(GetError, "glGetError");
    L(Viewport, "glViewport");               L(ClearColor, "glClearColor");
    L(Clear, "glClear");                     L(GenTextures, "glGenTextures");
    L(DeleteTextures, "glDeleteTextures");   L(BindTexture, "glBindTexture");
    L(TexParameteri, "glTexParameteri");     L(TexImage2D, "glTexImage2D");
    L(TexSubImage2D, "glTexSubImage2D");     L(PixelStorei, "glPixelStorei");
    L(ReadPixels, "glReadPixels");           L(DrawArrays, "glDrawArrays");
    L(CreateShader, "glCreateShader");       L(ShaderSource, "glShaderSource");
    L(CompileShader, "glCompileShader");     L(GetShaderiv, "glGetShaderiv");
    L(GetShaderInfoLog, "glGetShaderInfoLog"); L(DeleteShader, "glDeleteShader");
    L(CreateProgram, "glCreateProgram");     L(AttachShader, "glAttachShader");
    L(LinkProgram, "glLinkProgram");         L(GetProgramiv, "glGetProgramiv");
    L(GetProgramInfoLog, "glGetProgramInfoLog"); L(UseProgram, "glUseProgram");
    L(DeleteProgram, "glDeleteProgram");     L(GetUniformLocation, "glGetUniformLocation");
    L(Uniform1i, "glUniform1i");             L(Uniform2f, "glUniform2f");
    L(GenVertexArrays, "glGenVertexArrays"); L(BindVertexArray, "glBindVertexArray");
    L(DeleteVertexArrays, "glDeleteVertexArrays"); L(ActiveTexture, "glActiveTexture");
#undef L
    if (!ok) {
        fprintf(stderr, "[GL] missing OpenGL 3.3 entry points\n");
        return false;
    }
    const char *ver = (const char *)g.GetString(GL_VERSION_);
    const char *ren = (const char *)g.GetString(GL_RENDERER_);
    /* Desktop "3.3 ..." or newer (skip any "OpenGL ES " prefix check: ES is not supported) */
    int major = (ver != NULL) ? (ver[0] - '0') : 0;
    int minor = (ver != NULL && ver[1] == '.') ? (ver[2] - '0') : 0;
    if (major < 3 || (major == 3 && minor < 3)) {
        fprintf(stderr, "[GL] OpenGL %s < 3.3\n", ver != NULL ? ver : "?");
        return false;
    }
    snprintf(g.renderer, sizeof(g.renderer), "OpenGL %.24s | %.100s", ver, ren != NULL ? ren : "?");

    GLuint_ vs = compile_shader(GL_VERTEX_SHADER_, k_vertex_src);
    GLuint_ fs = compile_shader(GL_FRAGMENT_SHADER_, k_fragment_src);
    if (vs == 0u || fs == 0u) {
        return false;
    }
    g.program = g.CreateProgram();
    g.AttachShader(g.program, vs);
    g.AttachShader(g.program, fs);
    g.LinkProgram(g.program);
    g.DeleteShader(vs);
    g.DeleteShader(fs);
    GLint_ linked = 0;
    g.GetProgramiv(g.program, GL_LINK_STATUS_, &linked);
    if (!linked) {
        char log[1024];
        g.GetProgramInfoLog(g.program, (GLsizei_)sizeof(log), NULL, log);
        fprintf(stderr, "[GL] program link failed:\n%s\n", log);
        return false;
    }
    g.u_tex  = g.GetUniformLocation(g.program, "u_tex");
    g.u_src  = g.GetUniformLocation(g.program, "u_src");
    g.u_dst  = g.GetUniformLocation(g.program, "u_dst");
    g.u_mode = g.GetUniformLocation(g.program, "u_mode");

    g.GenVertexArrays(1, &g.vao);
    g.GenTextures(1, &g.tex);
    g.BindTexture(GL_TEXTURE_2D_, g.tex);
    g.TexParameteri(GL_TEXTURE_2D_, GL_TEXTURE_MIN_FILTER_, GL_LINEAR_);
    g.TexParameteri(GL_TEXTURE_2D_, GL_TEXTURE_MAG_FILTER_, GL_LINEAR_);
    g.TexParameteri(GL_TEXTURE_2D_, GL_TEXTURE_WRAP_S_, GL_CLAMP_TO_EDGE_);
    g.TexParameteri(GL_TEXTURE_2D_, GL_TEXTURE_WRAP_T_, GL_CLAMP_TO_EDGE_);
    g.PixelStorei(GL_UNPACK_ALIGNMENT_, 4);
    while (g.GetError() != GL_NO_ERROR_) {
    }
    g.ready = true;
    return true;
}

void c3gl_capture_next(uint32_t *dst) {
    g.capture = dst;
}

const char *c3gl_renderer(void) {
    return g.renderer;
}

const char *c3gl_filter_name(C3GlFilter filter) {
    switch (filter) {
        case C3GL_SHARP:   return "Sharp-Bilinear";
        case C3GL_NEAREST: return "Nearest";
        case C3GL_CRT:     return "CRT";
        default:           return "?";
    }
}

void c3gl_present(const uint32_t *argb, int src_w, int src_h, int win_w, int win_h,
                  bool integer_scale, C3GlFilter filter) {
    if (!g.ready || win_w <= 0 || win_h <= 0) {
        return;
    }
    /* Square-pixel 8:7 placement, optionally snapped to an integer multiple */
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

    g.Viewport(0, 0, win_w, win_h);
    g.ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    g.Clear(GL_COLOR_BUFFER_BIT_);

    g.ActiveTexture(GL_TEXTURE0_);
    g.BindTexture(GL_TEXTURE_2D_, g.tex);
    if (g.tex_w != src_w || g.tex_h != src_h) {
        g.TexImage2D(GL_TEXTURE_2D_, 0, GL_RGBA8_, src_w, src_h, 0, GL_BGRA_, GL_UNSIGNED_INT_8_8_8_8_REV_, argb);
        g.tex_w = src_w;
        g.tex_h = src_h;
    } else {
        g.TexSubImage2D(GL_TEXTURE_2D_, 0, 0, 0, src_w, src_h, GL_BGRA_, GL_UNSIGNED_INT_8_8_8_8_REV_, argb);
    }

    g.Viewport(dst_x, win_h - dst_y - dst_h, dst_w, dst_h);
    g.UseProgram(g.program);
    g.Uniform1i(g.u_tex, 0);
    g.Uniform2f(g.u_src, (GLfloat_)src_w, (GLfloat_)src_h);
    g.Uniform2f(g.u_dst, (GLfloat_)dst_w, (GLfloat_)dst_h);
    g.Uniform1i(g.u_mode, (GLint_)filter);
    g.BindVertexArray(g.vao);
    g.DrawArrays(GL_TRIANGLES_, 0, 3);

    if (g.capture != NULL) {
        /* Back-buffer readback (bottom-up) -> top-down ARGB for --gl-selftest */
        g.PixelStorei(GL_PACK_ALIGNMENT_, 4);
        g.ReadPixels(0, 0, win_w, win_h, GL_BGRA_, GL_UNSIGNED_INT_8_8_8_8_REV_, g.capture);
        for (int y = 0; y < win_h / 2; ++y) {
            uint32_t *a = g.capture + (size_t)y * (size_t)win_w;
            uint32_t *b = g.capture + (size_t)(win_h - 1 - y) * (size_t)win_w;
            for (int x = 0; x < win_w; ++x) {
                uint32_t t = a[x];
                a[x] = b[x];
                b[x] = t;
            }
        }
        g.capture = NULL;
    }
}

void c3gl_shutdown(void) {
    if (!g.ready) {
        return;
    }
    if (g.tex != 0u) g.DeleteTextures(1, &g.tex);
    if (g.vao != 0u) g.DeleteVertexArrays(1, &g.vao);
    if (g.program != 0u) g.DeleteProgram(g.program);
    memset(&g, 0, sizeof(g));
}
