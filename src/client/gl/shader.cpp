#include "shader.h"
#include "gl_errors.h"
#include <common/util.h>
#include <stdexcept>

#include <iostream>

namespace {
    class shader_compilation_error : public std::runtime_error {
      public:
        shader_compilation_error(const std::string& err)
            : std::runtime_error(err)
        {
        }
    };
    class shader_linkage_error : public std::runtime_error {
      public:
        shader_linkage_error(const std::string& err)
            : std::runtime_error(err)
        {
        }
    };
    GLuint compileShader(const std::string_view source, GLenum shaderType)
    {
        auto shaderID = glCheck(glCreateShader(shaderType));

        const GLchar* const shaderSourcePtr = source.data();
        const GLint shaderSourceLength = source.length();
        glCheck(glShaderSource(shaderID, 1, &shaderSourcePtr, &shaderSourceLength));
        glCheck(glCompileShader(shaderID));

        GLint logLength;

        glCheck(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength));
        if (logLength) {
            std::string infoLog(logLength, 0);
            glCheck(glGetShaderInfoLog(shaderID, logLength, nullptr, infoLog.data()));

            throw shader_compilation_error(infoLog);
        }

        return shaderID;
    }

    GLuint linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID)
    {
        auto id = glCheck(glCreateProgram());

        glCheck(glAttachShader(id, vertexShaderID));
        glCheck(glAttachShader(id, fragmentShaderID));

        glCheck(glLinkProgram(id));

        glCheck(glDetachShader(id, fragmentShaderID));
        glCheck(glDetachShader(id, vertexShaderID));

        GLint logLength;

        glCheck(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength));
        if (logLength) {
            std::string infoLog(logLength, 0);
            glCheck(glGetProgramInfoLog(id, logLength, nullptr, infoLog.data()));
            throw shader_linkage_error(infoLog);
        }

        return id;
    }

    struct ShaderStage {
        const GLuint shaderID;
        ShaderStage(const std::string_view shaderPath, const GLenum shaderType)
            : shaderID(compileShader(loadFileContents(shaderPath), shaderType))
        {
        }
        ~ShaderStage()
        {
            glCheck(glDeleteShader(shaderID));
        }
    };
} // namespace

namespace gl {

    Shader::~Shader()
    {
        destroy();
    }

    Shader::Shader(Shader&& other)
    {
        *this = std::move(other);
    }

    Shader& Shader::operator=(Shader&& other)
    {
        destroy();
        m_handle = other.m_handle;
        other.m_handle = 0;
        return *this;
    }

    void Shader::create(const std::string_view vertexFile,
                        const std::string_view fragmentFile)
    {
        glCheck(glUseProgram(0));
        const std::string vertFileFull("shaders/" + std::string(vertexFile) +
                                       "_vertex.glsl");
        const std::string fragFileFull("shaders/" + std::string(fragmentFile) +
                                       "_fragment.glsl");

        try {
            const ShaderStage vertexShader(vertFileFull, GL_VERTEX_SHADER);
            const ShaderStage fragmentShader(fragFileFull, GL_FRAGMENT_SHADER);
            m_handle = linkProgram(vertexShader.shaderID, fragmentShader.shaderID);
        }
        catch (const shader_compilation_error& e) {
            throw std::runtime_error("Shader " + vertFileFull + " failed to compile:\n" +
                                     e.what());
        }
        catch (const shader_linkage_error& e) {
            throw std::runtime_error("Linking failed for shaders " + vertFileFull +
                                     " and " + fragFileFull + ", with reason:\n" +
                                     e.what());
        }
    }

    void Shader::destroy()
    {
        glCheck(glDeleteProgram(m_handle));
        m_handle = 0;
    }

    void Shader::bind() const
    {
        glCheck(glUseProgram(m_handle));
    }

    UniformLocation Shader::getUniformLocation(const char* name)
    {
        UniformLocation location;
        location.ptr = glCheck(glGetUniformLocation(m_handle, name));
        return location;
    }

    void loadUniform(UniformLocation location, const glm::vec3& vector)
    {
        glCheck(glUniform3fv(location.ptr, 1, glm::value_ptr(vector)));
    }

    void loadUniform(UniformLocation location, const glm::ivec3& vector)
    {
        glCheck(glUniform3iv(location.ptr, 1, glm::value_ptr(vector)));
    }

    void loadUniform(UniformLocation location, const glm::mat4& matrix)
    {
        glCheck(glUniformMatrix4fv(location.ptr, 1, GL_FALSE, glm::value_ptr(matrix)));
    }

    void loadUniform(UniformLocation location, GLint value)
    {
        glCheck(glUniform1i(location.ptr, value));
    }

    void loadUniform(UniformLocation location, GLuint value)
    {
        glCheck(glUniform1ui(location.ptr, value));
    }

    void loadUniform(UniformLocation location, GLfloat value)
    {
        glCheck(glUniform1f(location.ptr, value));
    }

} // namespace gl
