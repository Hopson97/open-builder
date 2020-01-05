#pragma once

#include <array>
#include <glad/glad.h>
#include <string>

namespace gl {

/**
 * @brief Wrapper for an OpenGL cube-mapped texture object
 */
class CubeTexture final {
  public:
    void create(const std::array<std::string, 6> &textures);
    void destroy();
    void bind() const;

  private:
    GLuint m_handle = 0;
};

/**
 * @brief Wrapper for a regaulr OpenGL 2D texture
 */
class Texture2d final {
  public:
    void create(const std::string &file);
    void destroy();
    void bind() const;

  private:
    GLuint m_handle = 0;
};

} // namespace gl