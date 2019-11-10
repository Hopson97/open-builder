#include "texture2d.h"

#include "gl_errors.h"
namespace client {
    Texture2D::Texture2D(const std::string &texName)
    {
        create(texName);
    }

    Texture2D &Texture2D::operator=(Texture2D &&other)
    {
        m_textureId = other.m_textureId;
        other.m_textureId = 0;
        return *this;
    }

    Texture2D::Texture2D(Texture2D &&other)
        : m_textureId(other.m_textureId)
    {
        other.m_textureId = 0;
    }

    Texture2D::~Texture2D()
    {
        glCheck(glDeleteTextures(1, &m_textureId));
    }

    sf::Image Texture2D::create(const std::string &texName)
    {
        sf::Image image;
        image.loadFromFile("res/textures/" + texName);
        image.flipVertically();

        glCheck(glGenTextures(1, &m_textureId));
        bind();

        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x,
                             image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                             image.getPixelsPtr()));

        glCheck(glGenerateMipmap(GL_TEXTURE_2D));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                GL_LINEAR_MIPMAP_LINEAR));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        glCheck(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4));

        return image;
    }

    void Texture2D::bind() const
    {
        glCheck(glBindTexture(GL_TEXTURE_2D, m_textureId));
        glCheck(glActiveTexture(GL_TEXTURE0));
    }
} // namespace client