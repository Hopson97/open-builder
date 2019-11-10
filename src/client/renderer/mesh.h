#pragma once

#include <glad/glad.h>
#include <vector>

namespace client {
    struct Mesh {
        std::vector<GLfloat> vertices;
        std::vector<GLfloat> normals;
        std::vector<GLfloat> textureCoords;
        std::vector<GLuint> indices;
    };
} // namespace client