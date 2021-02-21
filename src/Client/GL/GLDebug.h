#pragma once

//#define DEBUG_GL_ERRORS

// clang-format off

// This is OpenGL checking code from
// https://github.com/SFML/SFML/blob/master/src/SFML/Graphics/GLCheck.hpp
#ifdef DEBUG_GL_ERRORS
#define glCheck(expr) expr; if(!GLAD_GL_KHR_debug){ glCheckError(__FILE__, __LINE__, #expr) };
#else
#define glCheck(call) call
#endif

void glCheckError(const char* file, unsigned int line,
    const char* expression);
void initGLDebug();

// clang-format on
