#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace glpp {
    enum class ShaderType {
        Vertex = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER,
    };

    struct UniformLocation final {
        GLint location = 0;
    };

    class Shader final {
      public:
        Shader() = default;
        Shader& operator=(Shader&& other) noexcept;
        Shader(Shader&& other) noexcept;
        ~Shader();

        Shader& operator=(Shader& other) = delete;
        Shader(Shader& other) = delete;

        void bind();
        void addShader(const std::string& filename, ShaderType type);
        void linkShaders();

        UniformLocation getUniformLocation(const char* name);

      private:
        std::vector<GLuint> m_stages;
        GLuint m_program = 0;
    };

    void loadUniform(UniformLocation location, int value);
    void loadUniform(UniformLocation location, const glm::mat4& matrix);
    void loadUniform(UniformLocation location, const glm::vec3& vector);

} // namespace glpp