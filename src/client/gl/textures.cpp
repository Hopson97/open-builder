#include "textures.h"
#include "gl_errors.h"
#include <common/debug.h>
#include <iostream>

#include <common/types.h>

namespace {
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

//
//  Cube Texture
//
/*
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
    m_handle = 0;
}

void CubeTexture::bind() const
{
    glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle));
}
*/

//
//  Texture 2D
//
void Texture2d::create(const std::string &file)
{
    m_handle = createTexture();
    bind();

    auto path = "res/" + file + ".png";
    bufferImage(GL_TEXTURE_2D, path);

    glCheck(glGenerateMipmap(GL_TEXTURE_2D));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR_MIPMAP_LINEAR));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    glCheck(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f));

    m_hasTexture = true;
}

void Texture2d::create(unsigned int width, unsigned int height,
                       const sf::Uint8 *pixels)
{
    m_handle = createTexture();
    bind();

    sf::Image img;
    img.create(width, height, pixels);
    glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x,
                         img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                         img.getPixelsPtr()));

    glCheck(glGenerateMipmap(GL_TEXTURE_2D));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR_MIPMAP_LINEAR));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    glCheck(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f));

    m_hasTexture = true;
}

void Texture2d::destroy()
{
    destroyTexture(&m_handle);
}

void Texture2d::bind() const
{
    glCheck(glBindTexture(GL_TEXTURE_2D, m_handle));
}

bool Texture2d::textureExists() const
{
    return m_hasTexture;
}

sf::Image loadRawImageFile(const std::string &file)
{
    sf::Image img;
    auto path = "res/" + file + ".png";

    if (!img.loadFromFile(path)) {
        std::cerr << "Could not load: " << file << '\n';
        return sf::Image();
    }

    return img;
}

//
//  Texture Array
//
void TextureArray::create(GLsizei numTextures, GLsizei textureSize)
{
    m_handle = createTexture();
    bind();

    m_maxTextures = numTextures;
    m_textureSize = textureSize;

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, textureSize, textureSize,
                 numTextures, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

GLuint TextureArray::addTexture(const std::string &file)
{
    sf::Image image;
    if (!image.loadFromFile("res/" + file + ".png")) {
        // Create a error image
        image.create(m_textureSize, m_textureSize);
        for (GLuint y = 0; y < m_textureSize; y++) {
            for (GLuint x = 0; x < m_textureSize; x++) {
                u8 r = static_cast<u8>(rand() % 255);
                u8 g = static_cast<u8>(rand() % 255);
                u8 b = static_cast<u8>(rand() % 255);
                image.setPixel(x, y, {r, g, b});
            }
        }
    };

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, m_textureCount, m_textureSize,
                    m_textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE,
                    image.getPixelsPtr());

    // Generate Mipmap
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_LOD_BIAS, -1);

    return m_textureCount++;
}

void TextureArray::destroy()
{
    destroyTexture(&m_handle);
    m_textureCount = 0;
    m_maxTextures = 0;
    m_textureSize = 0;
}

void TextureArray::bind() const
{
    glCheck(glBindTexture(GL_TEXTURE_2D_ARRAY, m_handle));
}

} // namespace gl