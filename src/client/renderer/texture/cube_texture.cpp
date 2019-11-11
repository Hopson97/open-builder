#include "cube_texture.h"

#include <SFML/Graphics/Image.hpp>

#include "../gl/gl_errors.h"

namespace client {
    CubeTexture::CubeTexture(const std::array<std::string, 6> &files)
    {
        loadFromFiles(files);
    }

    CubeTexture::CubeTexture(CubeTexture &&other)
    {
        m_textureId = other.m_textureId;
        other.m_textureId = 0;
    }

    CubeTexture &CubeTexture::operator=(CubeTexture &&other)
    {
        m_textureId = other.m_textureId;
        other.m_textureId = 0;
        return *this;
    }

    CubeTexture::~CubeTexture()
    {
        glCheck(glDeleteTextures(1, &m_textureId));
    }

    void CubeTexture::loadFromFiles(const std::array<std::string, 6> &files)
    {
        glCheck(glGenTextures(1, &m_textureId));
        glCheck(glActiveTexture(GL_TEXTURE0));
        glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId));

        for (int i = 0; i < 6; i++) {
            auto &str = files[i];
            sf::Image image;
            if (!image.loadFromFile("res/textures/skies/" + str + ".png")) {
                throw std::runtime_error("Unable to load CubeTexture Part: " +
                                         str);
            }

            auto param = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
            auto width = image.getSize().x;
            auto height = image.getSize().y;

            glCheck(glTexImage2D(param, 0, GL_RGBA, width, height, 0, GL_RGBA,
                                 GL_UNSIGNED_BYTE, image.getPixelsPtr()));
        }

        glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                                GL_LINEAR));
        glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,
                                GL_LINEAR));

        glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                                GL_CLAMP_TO_EDGE));
        glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                                GL_CLAMP_TO_EDGE));
    }

    void CubeTexture::bind() const
    {
        glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId));
    }

} // namespace client