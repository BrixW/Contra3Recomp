#ifndef CONTRA3_GL_PRESENT_H
#define CONTRA3_GL_PRESENT_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Portable OpenGL 3.3 Core presenter (src/gl_present.c): uploads the 256x224 ARGB frame to
 * one texture and draws a full-screen triangle through the Sharp-Bilinear / Nearest / CRT
 * GLSL filters. Every GL entry point, including GL 1.1 ones, is resolved through the
 * caller's loader (wglGetProcAddress + opengl32, SDL_GL_GetProcAddress, ...), so no platform
 * GL headers or import libraries are needed. A context must be current; the caller swaps.
 */

typedef enum C3GlFilter { C3GL_SHARP = 0, C3GL_NEAREST = 1, C3GL_CRT = 2, C3GL_FILTER_COUNT } C3GlFilter;

typedef void *(*C3GlGetProc)(const char *name);

bool        c3gl_init(C3GlGetProc get_proc);
void        c3gl_present(const uint32_t *argb, int src_w, int src_h, int win_w, int win_h,
                         bool integer_scale, C3GlFilter filter);
/* Read the next presented frame back into dst (win_w * win_h ARGB, top-down) before the swap */
void        c3gl_capture_next(uint32_t *dst);
const char *c3gl_renderer(void);
const char *c3gl_filter_name(C3GlFilter filter);
void        c3gl_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* CONTRA3_GL_PRESENT_H */
