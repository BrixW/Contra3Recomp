/*
 * Contra III: The Alien Wars (USA) - OpenGL presentation on Win32 (WGL context + swap)
 *
 * Creates a 3.3 Core context (or a >= 3.3 compatibility context) through WGL and hands the
 * drawing to the portable presenter in src/gl_present.c. Only opengl32.dll is linked; GL 1.1
 * entry points come from opengl32's exports and newer ones from wglGetProcAddress, so the
 * GDI path remains a zero-risk fallback when no capable driver is present.
 */

#include "win32_gl.h"
#include "gl_present.h"

#include <stdio.h>
#include <string.h>

#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001

typedef HGLRC (WINAPI *PFN_wglCreateContextAttribsARB)(HDC, HGLRC, const int *);
typedef BOOL  (WINAPI *PFN_wglSwapIntervalEXT)(int);

static HGLRC                  g_rc;
static HMODULE                g_opengl32;
static PFN_wglSwapIntervalEXT g_swap_interval;

static void *wgl_get_proc(const char *name) {
    PROC p = wglGetProcAddress(name);
    /* Some ICDs return small sentinel values instead of NULL for unsupported names */
    if (p == NULL || p == (PROC)1 || p == (PROC)2 || p == (PROC)3 || p == (PROC)-1) {
        p = (g_opengl32 != NULL) ? (PROC)GetProcAddress(g_opengl32, name) : NULL;
    }
    void *out;
    memcpy(&out, &p, sizeof(out));
    return out;
}

static void destroy_context(void) {
    if (g_rc != NULL) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(g_rc);
        g_rc = NULL;
    }
    g_swap_interval = NULL;
}

bool win32_gl_init(HWND hwnd, HDC hdc, bool vsync) {
    (void)hwnd;
    g_opengl32 = GetModuleHandleA("opengl32.dll");
    if (g_opengl32 == NULL) {
        g_opengl32 = LoadLibraryA("opengl32.dll");
    }

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
    void *p = wgl_get_proc("wglCreateContextAttribsARB");
    memcpy(&create_attribs, &p, sizeof(create_attribs));
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
    g_rc = boot;
    p = wgl_get_proc("wglSwapIntervalEXT");
    memcpy(&g_swap_interval, &p, sizeof(g_swap_interval));

    if (!c3gl_init(wgl_get_proc)) {
        fprintf(stderr, "[GL] OpenGL 3.3 presenter unavailable; using GDI\n");
        destroy_context();
        return false;
    }
    win32_gl_set_vsync(vsync);
    return true;
}

void win32_gl_set_vsync(bool on) {
    if (g_swap_interval != NULL) {
        g_swap_interval(on ? 1 : 0);
    }
}

bool win32_gl_vsync_supported(void) {
    return g_swap_interval != NULL;
}

void win32_gl_capture_next(uint32_t *dst) {
    c3gl_capture_next(dst);
}

const char *win32_gl_renderer(void) {
    return c3gl_renderer();
}

const char *win32_gl_filter_name(Win32GlFilter filter) {
    return c3gl_filter_name((C3GlFilter)filter);
}

void win32_gl_present(HDC hdc, const uint32_t *argb, int src_w, int src_h,
                      int win_w, int win_h, bool integer_scale, Win32GlFilter filter) {
    if (g_rc == NULL || win_w <= 0 || win_h <= 0) {
        return;
    }
    c3gl_present(argb, src_w, src_h, win_w, win_h, integer_scale, (C3GlFilter)filter);
    SwapBuffers(hdc);
}

void win32_gl_shutdown(HWND hwnd, HDC hdc) {
    (void)hwnd;
    (void)hdc;
    if (g_rc == NULL) {
        return;
    }
    c3gl_shutdown();
    destroy_context();
}
