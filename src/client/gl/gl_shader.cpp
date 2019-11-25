#include "gl_shader.h"
#include "gl_errors.h"
#include <common/file_io.h>
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

Shader createShader(const std::string &vertexFile,
                    const std::string &fragmentFile)
{
	glCheck(glUseProgram(0));
    std::string vertFileFull("shaders/" + vertexFile + "_vertex.glsl");
    std::string fragFileFull("shaders/" + fragmentFile + "_fragment.glsl");

    auto vertexSource = loadFileContents(vertFileFull);
    auto fragmentSource = loadFileContents(fragFileFull);

    auto vertexShaderID = compileShader(vertexSource.c_str(), GL_VERTEX_SHADER);
    auto fragmentShaderID =
        compileShader(fragmentSource.c_str(), GL_FRAGMENT_SHADER);

	Shader shader;
    shader.program = linkProgram(vertexShaderID, fragmentShaderID);

    glCheck(glDetachShader(shader.program, vertexShaderID));
    glCheck(glDetachShader(shader.program, fragmentShaderID));

    glCheck(glDeleteShader(vertexShaderID));
    glCheck(glDeleteShader(fragmentShaderID));

    return shader;
}

void destroyShader(Shader* shader)
{
    glCheck(glDeleteProgram(shader->program));
    shader->program = 0;
}

void useShader(Shader shader)
{
    glCheck(glUseProgram(shader.program));
}

UniformLocation getUniformLocation(Shader shader, const char *name)
{
    UniformLocation location;
    location.ptr = glCheck(glGetUniformLocation(shader.program, name));
    return location;
}

void loadUniform(UniformLocation location, const glm::vec3 &vector)
{
    glCheck(glUniform3fv(location.ptr, 1, glm::value_ptr(vector)));
}

void loadUniform(UniformLocation location, const glm::mat4 &matrix)
{
    glCheck(glUniformMatrix4fv(location.ptr, 1, GL_FALSE, glm::value_ptr(matrix)));
}
