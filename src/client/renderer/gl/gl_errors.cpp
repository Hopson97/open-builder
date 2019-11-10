#include "gl_errors.h"

#include <glad/glad.h>
#include <iostream>

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
