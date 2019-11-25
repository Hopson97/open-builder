#include "gl_textures.h"
#include "gl_errors.h"
#include <SFML/Graphics/Image.hpp>
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

        glCheck(glTexImage2D(param, 0, GL_RGBA, img.getSize().x,
                             img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                             img.getPixelsPtr()));
        return true;
    }

} // namespace

Texture createCubeTexture(const std::array<std::string, 6> &textures)
{
    Texture texture;
    texture.handle = createTexture();
    texture.type = TextureType::CubeMap;
    bindTexture(texture);

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

    return texture;
}

Texture createTexture2D(const std::string &file)
{
    Texture texture;
    texture.handle = createTexture();
    texture.type = TextureType::Texture2d;
    bindTexture(texture);

    auto path = TEXTURE_PATH + file + ".png";
    bufferImage(GL_TEXTURE_2D, path);

    return texture;
}

void bindTexture(const Texture &texture)
{
    glCheck(glBindTexture(static_cast<GLenum>(texture.type), texture.handle));
}

void destroyTexture(Texture *texture)
{
    glCheck(glDeleteTextures(1, &texture->handle));
    texture->handle = 0;
    texture->type = TextureType::None;
}
