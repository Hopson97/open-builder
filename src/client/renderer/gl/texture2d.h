#pragma once

#include <glad/glad.h>
#include <string>

#include <SFML/Graphics/Image.hpp>

namespace client {
    class Texture2D final {
      public:
        Texture2D() = default;
        Texture2D(const std::string &texName);

        Texture2D &operator=(Texture2D &&other);
        Texture2D(Texture2D &&other);

        Texture2D(const Texture2D &other) = delete;
        Texture2D &operator=(const Texture2D &other) = delete;

        ~Texture2D();

        sf::Image create(const std::string &texName);

        void bind() const;

      private:
        GLuint m_textureId;
    };
} // namespace client
