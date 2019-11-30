#include "gl_errors.h"
#include "gl_object.h"
#include <SFML/Graphics/Image.hpp>
#include <common/debug.h>
#include <iostream>

namespace {
const std::string TEXTURE_PATH = "res/textures/";

GLuint createTexture()
{
    GLuint handle;
    glCheck(glGenTextures(1, &handle));
    glCheck(glActiveTexture(GL_TEXTURE0));
    return handle;
}

bool bufferImage(GLenum param, const std::string &file)
{
    sf::Image img;
    if (!img.loadFromFile(file)) {
        std::cerr << "Could not load: " << file << '\n';
        return false;
    }

    glCheck(glTexImage2D(param, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr()));
    return true;
}

void destroyTexture(GLuint *texture)
{
    glCheck(glDeleteTextures(1, texture));
    *texture = 0;
}

} // namespace

namespace gl {

void CubeTexture::create(const std::array<std::string, 6> &textures)
{
    m_handle = createTexture();
    bind();

    for (int i = 0; i < 6; i++) {
        auto path = TEXTURE_PATH + "/skies/" + textures[i] + ".png";
        bufferImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, path);
    }

    glCheck(
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    glCheck(
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                            GL_CLAMP_TO_EDGE));
    glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                            GL_CLAMP_TO_EDGE));
}

void CubeTexture::destroy()
{
    destroyTexture(&m_handle);
    LOGVAR("Texture destroyed with ID: ", m_handle);
    m_handle = 0;
}

void CubeTexture::bind() const
{
    glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle));
}

void Texture2d::create(const std::string &file)
{
    m_handle = createTexture();
    bind();

    auto path = TEXTURE_PATH + file + ".png";
    bufferImage(GL_TEXTURE_2D, path);
    LOG("Texture created.");

    glCheck(glGenerateMipmap(GL_TEXTURE_2D));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR_MIPMAP_LINEAR));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    glCheck(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4));
}

void Texture2d::destroy()
{
    destroyTexture(&m_handle);
}

void Texture2d::bind() const
{
    glCheck(glBindTexture(GL_TEXTURE_2D, m_handle));
}

} // namespace gl