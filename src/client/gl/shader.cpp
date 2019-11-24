#include "shader.h"

#include "gl_errors.h"
#include <common/file_io.h>
#include <iostream>
#include <stdexcept>

namespace {
    GLuint compileShader(const GLchar *source, GLenum shaderType)
    {
        auto shaderID = glCheck(glCreateShader(shaderType));

        glCheck(glShaderSource(shaderID, 1, &source, nullptr));
        glCheck(glCompileShader(shaderID));

        GLint isSuccess = 0;
        GLchar infoLog[512];

        glCheck(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isSuccess));
        if (!isSuccess) {
            glCheck(glGetShaderInfoLog(shaderID, 512, nullptr, infoLog));
            throw std::runtime_error("Unable to load a shader: " +
                                     std::string(infoLog));
        }

        return shaderID;
    }

    GLuint linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID)
    {
        auto id = glCheck(glCreateProgram());

        glCheck(glAttachShader(id, vertexShaderID));
        glCheck(glAttachShader(id, fragmentShaderID));

        glCheck(glLinkProgram(id));

        GLint isSuccess = 0;
        GLchar infoLog[512];

        glCheck(glGetProgramiv(id, GL_LINK_STATUS, &isSuccess));
        if (!isSuccess) {
            glCheck(glGetProgramInfoLog(id, 512, nullptr, infoLog));
            throw std::runtime_error("Unable to link a shader: " +
                                     std::string(infoLog));
        }

        return id;
    }
} // namespace

namespace client {
    Shader::Shader(const std::string &vertexFile,
                   const std::string &fragmentFile)
    {
        load(vertexFile, fragmentFile);
    }

    Shader::Shader(Shader &&other)
    {
        m_program = other.m_program;
        other.m_program = 0;
    }

    Shader &Shader::operator=(Shader &&other)
    {
        m_program = other.m_program;
        other.m_program = 0;
        return *this;
    }

    Shader::~Shader()
    {
        glCheck(glDeleteProgram(m_program));
    }

    void Shader::load(const std::string &vertexFile,
                      const std::string &fragmentFile)
    {
        glCheck(glUseProgram(0));
        std::string vertFileFull("shaders/" + vertexFile + "_vertex.glsl");
        std::string fragFileFull("shaders/" + fragmentFile + "_fragment.glsl");

        auto vertexSource = loadFileContents(vertFileFull);
        auto fragmentSource = loadFileContents(fragFileFull);

        auto vertexShaderID =
            compileShader(vertexSource.c_str(), GL_VERTEX_SHADER);
        auto fragmentShaderID =
            compileShader(fragmentSource.c_str(), GL_FRAGMENT_SHADER);

        m_program = linkProgram(vertexShaderID, fragmentShaderID);

        glCheck(glDetachShader(m_program, vertexShaderID));
        glCheck(glDetachShader(m_program, fragmentShaderID));

        glCheck(glDeleteShader(vertexShaderID));
        glCheck(glDeleteShader(fragmentShaderID));
    }

    GLuint Shader::getUniformLocation(const char *name) const
    {
        return glCheck(glGetUniformLocation(m_program, name));
    }

    void Shader::use() const
    {
        glCheck(glUseProgram(m_program));
    }

    void loadVector3(GLuint location, const glm::vec3 &vector)
    {
        glCheck(glUniform3fv(location, 1, glm::value_ptr(vector)));
    }

    void loadMatrix4(GLuint location, const glm::mat4 &matrix)
    {
        glCheck(
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
    }
} // namespace client