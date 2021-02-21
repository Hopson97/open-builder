#include "Texture.h"

#include "GLDebug.h"
#include <SFML/Graphics/Image.hpp>
#include <stdexcept>

namespace glpp {

    glpp::Texture2d ::Texture2d()
    {
        glCheck(glGenTextures(1, &m_textureId));
    }

    Texture2d& Texture2d ::operator=(Texture2d&& other) noexcept
    {
        m_textureId = other.m_textureId;
        other.m_textureId = 0;
        return *this;
    }

    Texture2d ::Texture2d(Texture2d&& other) noexcept
        : m_textureId{other.m_textureId}
    {
        other.m_textureId = 0;
    }

    Texture2d ::~Texture2d()
    {
        if (m_textureId) {
            glCheck(glDeleteTextures(1, &m_textureId));
        }
    }

    void Texture2d::create(unsigned width, unsigned height)
    {
        bind();
        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                             GL_UNSIGNED_BYTE, nullptr));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    void Texture2d::create(const std::string& name, bool flip)
    {
        sf::Image image;
        if (!image.loadFromFile("data/textures/" + name)) {
            throw std::runtime_error("Could not load texture.");
        }
        if (flip) {
            image.flipVertically();
        }
        bind();
        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x,
                             image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                             image.getPixelsPtr()));
        glCheck(glGenerateMipmap(GL_TEXTURE_2D));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                GL_LINEAR_MIPMAP_LINEAR));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        glCheck(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f));
    }

    void Texture2d::bind() const
    {
        glBindTexture(GL_TEXTURE_2D, m_textureId);
    }

    GLuint Texture2d::textureId() const
    {
        return m_textureId;
    }

} // namespace glpp