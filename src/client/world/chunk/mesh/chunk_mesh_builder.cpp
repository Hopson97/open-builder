#include "chunk_mesh_builder.h"
#include "../../../renderer/model/mesh.h"
#include "../chunk.h"

#include <iostream>

namespace {
    // clang-format off
    using FaceVertices = const std::array<GLfloat, 12>;

    FaceVertices frontFace {
        0, 0, 1,
        1, 0, 1,
        1, 1, 1,
        0, 1, 1,
    };

    FaceVertices backFace {
        1, 0, 0,
        0, 0, 0,
        0, 1, 0,
        1, 1, 0,
    };

    FaceVertices leftFace {
        0, 0, 0,
        0, 0, 1,
        0, 1, 1,
        0, 1, 0,
    };

    FaceVertices rightFace {
        1, 0, 1,
        1, 0, 0,
        1, 1, 0,
        1, 1, 1,
    };

    FaceVertices topFace {
        0, 1, 1,
        1, 1, 1,
        1, 1, 0,
        0, 1, 0,
    };

    FaceVertices bottomFace {
        0, 0, 0,
        1, 0, 0,
        1, 0, 1,
        0, 0, 1
    };

    FaceVertices xFace1 {
        0, 0, 0,
        1, 0, 1,
        1, 1, 1,
        0, 1, 0,
    };

    FaceVertices xFace2 {
        0, 0, 1,
        1, 0, 0,
        1, 1, 0,
        0, 1, 1,
    };
    // clang-format on
} // namespace

namespace client {

    ChunkMeshBuilder::ChunkMeshBuilder(const Chunk &chunk)
        : mp_chunk(chunk)
    {
    }

    ChunkMesh ChunkMeshBuilder::createMesh()
    {
        ChunkMesh chunkMesh;
        Mesh mesh;

        for (int y = 0; y < Chunk::SIZE; ++y) {
            for (int z = 0; z < Chunk::SIZE; ++z) {
                for (int x = 0; x < Chunk::SIZE; ++x) {
                    Block block = mp_chunk.getBlock({x, y, z});

                    Block left = mp_chunk.getBlock({x - 1, y, z});
                    Block right = mp_chunk.getBlock({x + 1, y, z});

                    Block up = mp_chunk.getBlock({x, y + 1, z});
                    Block down = mp_chunk.getBlock({x, y - 1, z});

                    Block forwards = mp_chunk.getBlock({x, y, z + 1});
                    Block back = mp_chunk.getBlock({x, y, z - 1});

                    if (left.type == BlockType::Air) {

                    }
                    

                    (void)block;
                    (void)left;
                    (void)right;
                    (void)up;
                    (void)down;
                    (void)forwards;
                    (void)back;
                }
            }
        }

        return chunkMesh;
    }
} // namespace client
