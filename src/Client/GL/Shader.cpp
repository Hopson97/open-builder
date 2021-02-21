#include "Shader.h"

#include "GLDebug.h"
#include <cassert>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

namespace {
    std::string loadFile(const std::string fName)
    {
        std::ifstream inFile(fName);
        if (!inFile.is_open()) {
            std::cerr << "Unable to open file " << fName << std::endl;
        }
        std::ostringstream stream;
        stream << inFile.rdbuf();
        return stream.str();
    }
} // namespace

namespace glpp {
    Shader& Shader::operator=(Shader&& other) noexcept
    {
        m_program = other.m_program;
        other.m_program = 0;
        return *this;
    }

    Shader::Shader(Shader&& other) noexcept
        : m_program{other.m_program}
    {
        other.m_program = 0;
    }

    Shader::~Shader()
    {
        if (m_program != 0) {
            glCheck(glDeleteProgram(m_program));
        }
    }

    void Shader::bind()
    {
        assert(m_program != 0);
        glCheck(glUseProgram(m_program));
    }

    void Shader::addShader(const std::string& filename, ShaderType type)
    {
        std::string ext = (type == ShaderType::Vertex ? ".vert" : ".frag");
        std::string source = loadFile("Data/Shaders/" + filename + ext);
        auto shaderId = glCheck(glCreateShader(static_cast<GLenum>(type)));

        const GLchar* const shaderSourcePtr = source.data();
        const GLint shaderSourceLength = source.length();
        glCheck(glShaderSource(shaderId, 1, &shaderSourcePtr, &shaderSourceLength));
        glCheck(glCompileShader(shaderId));

        GLint logLength;
        glCheck(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength));
        if (logLength) {
            std::string infoLog(logLength, 0);
            glCheck(glGetShaderInfoLog(shaderId, logLength, nullptr, infoLog.data()));
            std::cerr << "Error for " << ext << " shader: " << infoLog << std::endl;
            throw std::runtime_error(infoLog);
        }

        m_stages.push_back(shaderId);
    }

    void Shader::linkShaders()
    {
        m_program = glCheck(glCreateProgram());

        for (auto& stage : m_stages) {
            glCheck(glAttachShader(m_program, stage));
        }

        glCheck(glLinkProgram(m_program));

        for (auto& stage : m_stages) {
            glCheck(glDetachShader(m_program, stage));
        }

        GLint logLength;
        glCheck(glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength));
        if (logLength) {
            std::string infoLog(logLength, 0);
            glCheck(glGetProgramInfoLog(m_program, logLength, nullptr, infoLog.data()));
            throw std::runtime_error(infoLog);
        }
    }

    UniformLocation Shader::getUniformLocation(const char* name)
    {
        return {glCheck(glGetUniformLocation(m_program, name))};
    }

    void loadUniform(UniformLocation l, int value)
    {
        assert(l.location != -1);
        glCheck(glUniform1i(l.location, value));
    }

    void loadUniform(UniformLocation l, const glm::mat4& matrix)
    {
        assert(l.location != -1);
        glCheck(glUniformMatrix4fv(l.location, 1, GL_FALSE, glm::value_ptr(matrix)));
    }

    void loadUniform(UniformLocation l, const glm::vec3& vector)
    {
        assert(l.location != -1);
        glCheck(glUniform3fv(l.location, 1, glm::value_ptr(vector)));
    }

} // namespace glpp