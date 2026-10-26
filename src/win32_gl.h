#ifndef CONTRA3_WIN32_GL_H
#define CONTRA3_WIN32_GL_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Hardware OpenGL 3.3 Core presentation backend for the Win32 desktop runner.
 * Links only the system opengl32.dll; all GL 2.0+ entry points are resolved
 * at runtime through wglGetProcAddress, so the GDI path remains a zero-risk fallback.
 */

typedef enum Win32GlFilter {
    WIN32_GL_FILTER_SHARP = 0,   /* Sharp-bilinear: crisp pixels, smooth non-integer edges */
    WIN32_GL_FILTER_NEAREST,     /* Raw nearest-neighbour texel fetch */
    WIN32_GL_FILTER_CRT,         /* Gaussian scanlines + aperture-grille mask + soft curvature */
    WIN32_GL_FILTER_COUNT
} Win32GlFilter;

bool        win32_gl_init(HWND hwnd, HDC hdc, bool vsync);
void        win32_gl_present(HDC hdc, const uint32_t *argb, int src_w, int src_h,
                             int win_w, int win_h, bool integer_scale, Win32GlFilter filter);
void        win32_gl_set_vsync(bool on);
/* Read the next presented frame back into dst (win_w * win_h ARGB, top-down) before the swap */
void        win32_gl_capture_next(uint32_t *dst);
bool        win32_gl_vsync_supported(void);
const char *win32_gl_renderer(void);
const char *win32_gl_filter_name(Win32GlFilter filter);
void        win32_gl_shutdown(HWND hwnd, HDC hdc);

#endif /* CONTRA3_WIN32_GL_H */
