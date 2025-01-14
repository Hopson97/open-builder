#include "shader.h"
#include "gl_errors.h"
#include <common/util.h>
#include <stdexcept>

#include <iostream>

namespace
{
    enum class IVParameter
    {
        CompileStatus = GL_COMPILE_STATUS,
        LinkStatus = GL_LINK_STATUS
    };

    [[nodiscard]] constexpr auto to_string(IVParameter param)
    {
        switch (param)
        {
            case IVParameter::CompileStatus:
                return "compile";

            case IVParameter::LinkStatus:
                return "link";
        }
        return "ShouldNeverGetHere";
    }

    template <IVParameter parameter>
    [[nodiscard]] auto verify_shader(GLuint shader)
    {
        // Verify
        GLint status = 0;
        if constexpr (parameter == IVParameter::CompileStatus)
        {
            glGetShaderiv(shader, static_cast<GLenum>(parameter), &status);
        }
        else if constexpr (parameter == IVParameter::LinkStatus)
        {
            glGetProgramiv(shader, static_cast<GLenum>(parameter), &status);
        }
        else
        {
            std::println(std::cerr, "Unkown verify type for action '{}'.", to_string(parameter));
        }

        if (status == GL_FALSE)
        {
            GLsizei length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            if constexpr (parameter == IVParameter::CompileStatus)
            {
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            }
            else if constexpr (parameter == IVParameter::LinkStatus)
            {
                glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &length);
            }
            std::string buffer(1024, ' ');
            glGetShaderInfoLog(shader, 1024, NULL, buffer.data());
            std::println(std::cerr, "Failed to {} shader. Error: {}", to_string(parameter), buffer);
            return false;
        }
        return true;
    }

    [[nodiscard]] GLuint compile_shader(const char* source, GLuint shader_type)
    {
        GLuint shader = glCreateShader(shader_type);
        glShaderSource(shader, 1, (const GLchar* const*)&source, nullptr);
        glCompileShader(shader);

        if (!verify_shader<IVParameter::CompileStatus>(shader))
        {
            return 0;
        }
        return shader;
    }

} // namespace

namespace gl
{

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

    bool Shader::create(const std::string_view vertexFile, const std::string_view fragmentFile)
    {
        glCheck(glUseProgram(0));
        const std::string vertFileFull("assets/shaders/" + std::string(vertexFile) +
                                       "_vertex.glsl");
        const std::string fragFileFull("assets/shaders/" + std::string(fragmentFile) +
                                       "_fragment.glsl");

        return loadStage(vertFileFull, ShaderType::Vertex) &&
               loadStage(fragFileFull, ShaderType::Fragment) && linkStages();
    }

    bool Shader::loadStage(const std::string_view file_path, ShaderType shader_type)
    {
        // Load the files into strings and verify
        auto source = loadFileContents(file_path);
        if (source.length() == 0)
        {
            return false;
        }

        GLuint shader = compile_shader(source.c_str(), static_cast<GLenum>(shader_type));
        if (!shader)
        {
            return false;
        }
        m_stages.push_back(shader);

        return true;
    }

    bool Shader::linkStages()
    {
        // Link the shaders together and verify the link status
        m_handle = glCreateProgram();
        for (auto stage : m_stages)
        {
            glAttachShader(m_handle, stage);
        }
        glLinkProgram(m_handle);

        if (!verify_shader<IVParameter::LinkStatus>(m_handle))
        {
            std::println(std::cerr, "Failed to link shader.");

            return false;
        }
        glValidateProgram(m_handle);

        int status = 0;
        glGetProgramiv(m_handle, GL_VALIDATE_STATUS, &status);
        if (status == GL_FALSE)
        {
            std::println(std::cerr, "Failed to validate shader program.");
            return false;
        }

        // Delete the temporary shaders
        for (auto& shader : m_stages)
        {
            glDeleteShader(shader);
        }
        m_stages.clear();
        m_stages.shrink_to_fit();

        return true;
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
