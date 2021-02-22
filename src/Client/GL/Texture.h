#pragma once

#include <glad/glad.h>
#include <string>

namespace glpp {
    enum class TexParam { Linear };

    class Texture2d {
      public:
        Texture2d();
        Texture2d& operator=(Texture2d&& other) noexcept;
        Texture2d(Texture2d&& other) noexcept;
        ~Texture2d();

        void create(unsigned width, unsigned height);
        void create(const std::string& name, bool flip);
        void bind() const;
        GLuint textureId() const;

        Texture2d& operator=(Texture2d& other) = delete;
        Texture2d(Texture2d& other) = delete;

      private:
        GLuint m_handle = 0;
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
} // namespace glpp