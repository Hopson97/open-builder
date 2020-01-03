#pragma once

#include <glad/glad.h>
#include <string>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gl {
/**
 * @brief A uniform location in a shader
 */
struct UniformLocation final {
    GLuint ptr = 0;
};

/**
 * @brief Wrapper for a OpenGL shder object
 */
class Shader final {
  public:
    void create(const std::string &vertexFile, const std::string &fragmentFile);
    void destroy();
    void bind() const;

    UniformLocation getUniformLocation(const char *name);

  private:
    GLuint m_handle = 0;
};

// Functons for shaders
void loadUniform(UniformLocation location, const glm::vec3 &vector);
void loadUniform(UniformLocation location, const glm::mat4 &matrix);

} // namespace gl
