#pragma once

#include <SFML/Graphics/Image.hpp>
#include <array>
#include <glad/glad.h>
#include <string>

namespace gl {

    /**
     * @brief Wrapper for an OpenGL cube-mapped texture object
     */
    /*
    class CubeTexture final {
      public:
        void create(const std::array<std::string, 6> &textures);
        void destroy();
        void bind() const;

      private:
        GLuint m_handle = 0;
    };
    */

    /**
     * @brief Wrapper for a regaulr OpenGL 2D texture
     */
    class Texture2d final {
      public:
        Texture2d();
        ~Texture2d();

        Texture2d(Texture2d&& other);
        Texture2d& operator=(Texture2d&& other);

        Texture2d(const Texture2d&) = delete;
        Texture2d& operator=(const Texture2d&) = delete;

        GLuint createFramebufferTexture(unsigned width, unsigned height);
        void create(const sf::Image& image);
        void create(const std::string& file, bool flipImage);
        void create(unsigned int width, unsigned int height, const sf::Uint8* pixels);
        void destroy();
        void bind() const;
        bool textureExists() const;

      private:
        void reset();

        GLuint m_handle = 0;
        bool m_hasTexture = false;
    };

    class TextureArray final {
      public:
        TextureArray();
        ~TextureArray();

        TextureArray(TextureArray&& other);
        TextureArray& operator=(TextureArray&& other);

        TextureArray(const TextureArray&) = delete;
        TextureArray& operator=(const TextureArray&) = delete;

        void create(GLsizei numTextures, GLsizei textureSize);
        GLuint addTexture(const std::string& file);
        void destroy();
        void bind() const;

      private:
        void reset();

        GLuint m_handle = 0;
        GLuint m_textureCount = 0;
        GLuint m_maxTextures = 0;
        GLuint m_textureSize = 0;
    };

    sf::Image loadRawImageFile(const std::string& file);

} // namespace gl
