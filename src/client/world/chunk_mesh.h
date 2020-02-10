#pragma once

#include "../gl/vertex_array.h"
#include <array>
#include <common/world/coordinate.h>

struct MeshFace {
    std::array<GLbyte, 12> vertices;
    GLbyte lightLevel;
};

struct ChunkMesh {
    ChunkMesh(const ChunkPosition& chunkPosition);
    void addFace(const MeshFace& face, const BlockPosition& blockPosition,
                 GLuint texture);

    gl::VertexArray createBuffer();

    size_t calculateBufferSize() const;

    size_t vertexCount() const;

    std::vector<GLuint> vertexData;

    ChunkPosition position;
};

struct ChunkMeshCollection {
    ChunkMeshCollection(const ChunkPosition& chunkPosition);
    ChunkMesh blockMesh;
    ChunkMesh fluidMesh;
    ChunkMesh floraMesh;
};