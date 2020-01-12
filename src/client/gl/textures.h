#pragma once

#include <SFML/Graphics/Image.hpp>
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
    void create(unsigned int width, unsigned int height, const sf::Uint8* pixels);
    void destroy();
    void bind() const;
    bool textureExists() const;

  private:
    GLuint m_handle = 0;
    bool m_hasTexture = false;
};

class TextureArray final {
  public:
    void create(GLsizei numTextures, GLsizei textureSize);
    int addTexture(const std::string &file);
    void destroy();
    void bind() const;

private:
    GLuint m_handle = 0;
    GLuint m_textureCount = 0;
    GLuint m_maxTextures = 0;
    GLuint m_textureSize = 0;
};

sf::Image loadRawImageFile(const std::string& file);

} // namespace gl