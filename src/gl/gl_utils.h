#ifndef GL_UTILS_H
#define GL_UTILS_H

// Turn off to not assert.
#define USE_GL_ASSERT

#include "glad/glad.h"

#include <stdio.h>
#include <stdlib.h>

inline void handleGLError(int line, const char* file, const char* func)
{
    GLenum err = glGetError();

    switch (err) {
        case GL_NO_ERROR:
            return;
        case GL_INVALID_ENUM:
            fprintf(stderr, "%s: %s: %d: GL_INVALID_ENUM\n", file, func, line);
            break;
        case GL_INVALID_VALUE:
            fprintf(stderr, "%s: %s: %d: GL_INVALID_VALUE\n", file, func, line);
            break;
        case GL_INVALID_OPERATION:
            fprintf(
                stderr, "%s: %s: %d: GL_INVALID_OPERATION\n", file, func, line);
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            fprintf(stderr,
                    "%s: %s: %d: GL_INVALID_FRAMEBUFFER_OPERATION\n",
                    file,
                    func,
                    line);
            break;
        case GL_OUT_OF_MEMORY:
            fprintf(stderr, "%s: %s: %d: GL_OUT_OF_MEMORY\n", file, func, line);
            break;
        case GL_STACK_UNDERFLOW:
            fprintf(
                stderr, "%s: %s: %d: GL_STACK_UNDERFLOW\n", file, func, line);
            break;
        case GL_STACK_OVERFLOW:
            fprintf(
                stderr, "%s: %s: %d: GL_STACK_OVERFLOW\n", file, func, line);
            break;
    }
    fprintf(stderr, "Exiting...\n");
    exit(-1);
}

#ifdef USE_GL_ASSERT
#define GL(arg)                                                                \
    arg;                                                                       \
    handleGLError(__LINE__, __FILE__, __func__);
#else
#define GL(arg) arg
#endif

#endif // #ifndef GL_UTILS_H
