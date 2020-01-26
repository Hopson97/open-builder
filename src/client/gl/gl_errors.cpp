#include "gl_errors.h"

#include <glad/glad.h>
#include <iostream>

void GLAPIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id,
                                GLenum severity, GLsizei length,
                                const char *message, const void *)
{

    const char *sev = "";
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            sev = "\e[91m";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            sev = "\e[93m";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            sev = "\e[92m";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            sev = "\e[34m";
            break;
    }

    const char *src = "?";
    switch (source) {
        case GL_DEBUG_SOURCE_API:
            src = "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            src = "window system";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            src = "shader compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            src = "third party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            src = "app";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            src = "other";
            break;
    }

    const char *type_str = "?";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            type_str = "error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            type_str = "deprecated behavior";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            type_str = "undefined behavior";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            type_str = "portability";
            break;
        case GL_DEBUG_TYPE_MARKER:
            type_str = "marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            type_str = "push group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            type_str = "pop group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            type_str = "other";
            break;
    }

    fprintf(stderr, "debug:%s type: %s, source: %s, message: \"%.*s\"\e[0m\n",
            sev, type_str, src, length, message);
}

void initGLDebug()
{
#ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // disable if in release
#endif
    glDebugMessageCallback(glDebugCallback, nullptr);

    // this disables messages printed for successfully compiled shaders
    glDebugMessageControl(GL_DEBUG_SOURCE_SHADER_COMPILER, GL_DEBUG_TYPE_OTHER,
                          GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
}

void glCheckError(const char *file, unsigned int line, const char *expression)
{
    // Get the last error
    GLenum errorCode = glGetError();

    while (errorCode != GL_NO_ERROR) {
        std::string fileString = file;
        std::string error = "Unknown error";
        std::string description = "No description";

        // Decode the error code
        switch (errorCode) {
            case GL_INVALID_ENUM: {
                error = "GL_INVALID_ENUM";
                description = "An unacceptable value has been specified "
                              "for an enumerated argument.";
                break;
            }

            case GL_INVALID_VALUE: {
                error = "GL_INVALID_VALUE";
                description = "A numeric argument is out of range.";
                break;
            }

            case GL_INVALID_OPERATION: {
                error = "GL_INVALID_OPERATION";
                description = "The specified operation is not allowed in "
                              "the current state.";
                break;
            }

            case GL_OUT_OF_MEMORY: {
                error = "GL_OUT_OF_MEMORY";
                description = "There is not enough memory left to execute "
                              "the command.";
                break;
            }
        }

        // Log the error
        std::cerr << "An internal OpenGL call failed in "
                  << fileString.substr(fileString.find_last_of("\\/") + 1)
                  << "(" << line << ")."
                  << "\nExpression:\n   " << expression
                  << "\nError description:\n   " << error << "\n   "
                  << description << "\n"
                  << std::endl;
        glCheckError(file, line, expression);
        exit(-1);
    }
}
