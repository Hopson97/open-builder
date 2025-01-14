#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include <string_view>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gl
{
    /**
     * @brief A uniform location in a shader
     */
    struct UniformLocation final
    {
        GLuint ptr = 0;
    };

    /**
     * @brief Wrapper for a OpenGL shder object
     */
    class Shader final
    {
        enum class ShaderType
        {
            Vertex = GL_VERTEX_SHADER,
            Fragment = GL_FRAGMENT_SHADER,
        };

      public:
        Shader() = default;
        ~Shader();

        Shader(Shader&& other);
        Shader& operator=(Shader&& other);

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        bool create(const std::string_view vertexFile, const std::string_view fragmentFile);
        void destroy();
        void bind() const;

        UniformLocation getUniformLocation(const char* name);

      private:
        bool loadStage(const std::string_view file, ShaderType shader_type);
        bool linkStages();
        GLuint m_handle = 0;
        std::vector<GLuint> m_stages;
    };

    // Functons for shaders
    void loadUniform(UniformLocation location, const glm::ivec3& vector);
    void loadUniform(UniformLocation location, const glm::vec3& vector);
    void loadUniform(UniformLocation location, const glm::mat4& matrix);

    void loadUniform(UniformLocation location, GLint value);
    void loadUniform(UniformLocation location, GLuint value);
    void loadUniform(UniformLocation location, GLfloat value);

} // namespace gl
