#pragma once

#include "../gl/gl_object.h"
#include <common/world/coordinate.h>

struct MeshFace {
    std::array<GLfloat, 12> vertices;
    GLfloat lightLevel;
};

struct ChunkMesh {

    void addFace(const MeshFace &face, const BlockPosition &blockPosition);

    gl::VertexArray createBuffer();

    std::vector<float> vertices;
    std::vector<GLfloat> textureCoords;
    std::vector<GLuint> indices;
    std::vector<GLfloat> cardinalLights;
    int indicesCount = 0;

    ChunkPosition position;
};