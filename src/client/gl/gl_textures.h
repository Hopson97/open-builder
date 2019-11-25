#pragma once

#include <array>
#include <glad/glad.h>

enum class TextureType {
    None,
    CubeMap = GL_TEXTURE_CUBE_MAP,
    Texture2d = GL_TEXTURE_2D,
};

/**
        Some structs that hold texture handles for type safety
*/
struct Texture {
    GLuint handle = 0;
    TextureType type = TextureType::None;
};

Texture createCubeTexture(const std::array<std::string, 6> &textures);
Texture createTexture2D(const std::string &file);

void bindTexture(const Texture &texture);
void destroyTexture(Texture *texture);