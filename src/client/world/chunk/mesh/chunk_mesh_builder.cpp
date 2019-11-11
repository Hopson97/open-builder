#include "chunk_mesh_builder.h"
#include "../../../renderer/model/mesh.h"
#include "../chunk.h"

#include <iostream>

namespace {
    // clang-format off
    const client::BlockFace frontFace {{
        0, 0, 1,
        1, 0, 1,
        1, 1, 1,
        0, 1, 1,
    },
    {
        0, 0, 1,
    }};

    const client::BlockFace backFace {{
        1, 0, 0,
        0, 0, 0,
        0, 1, 0,
        1, 1, 0,
    },
    {
        0, 0, -1
    }};

    const client::BlockFace leftFace {{
        0, 0, 0,
        0, 0, 1,
        0, 1, 1,
        0, 1, 0,
    },
    {
        -1, 0, 0
    }};

    const client::BlockFace rightFace {{
        1, 0, 1,
        1, 0, 0,
        1, 1, 0,
        1, 1, 1,
    },
    {
        1, 0, 0
    }};

    const client::BlockFace topFace {{
        0, 1, 1,
        1, 1, 1,
        1, 1, 0,
        0, 1, 0,
    },
    {
        0, 1, 0
    }};

    const client::BlockFace bottomFace {{
        0, 0, 0,
        1, 0, 0,
        1, 0, 1,
        0, 0, 1
    },
    {
        0, -1, 0
    }};

/*
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
*/
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

        for (int y = 0; y < Chunk::SIZE; ++y) {
            for (int z = 0; z < Chunk::SIZE; ++z) {
                for (int x = 0; x < Chunk::SIZE; ++x) {
                    Block block = mp_chunk.getBlock({x, y, z});

                    // Eventually, this will be used to determine the mesh type
                    // and texture For now, it is not needed
                    (void)block;

                    Block left = mp_chunk.getBlock({x - 1, y, z});
                    Block right = mp_chunk.getBlock({x + 1, y, z});

                    Block up = mp_chunk.getBlock({x, y + 1, z});
                    Block down = mp_chunk.getBlock({x, y - 1, z});

                    Block forwards = mp_chunk.getBlock({x, y, z + 1});
                    Block back = mp_chunk.getBlock({x, y, z - 1});

                    if (left.type == BlockType::Air) {
                        addFace(leftFace, {x, y, z});
                    }
                    if (right.type == BlockType::Air) {
                        addFace(rightFace, {x, y, z});
                    }
                    if (up.type == BlockType::Air) {
                        addFace(topFace, {x, y, z});
                    }
                    if (down.type == BlockType::Air) {
                        addFace(bottomFace, {x, y, z});
                    }
                    if (forwards.type == BlockType::Air) {
                        addFace(frontFace, {x, y, z});
                    }
                    if (back.type == BlockType::Air) {
                        addFace(backFace, {x, y, z});
                    }
                }
            }
        }

        chunkMesh.solidBlocks.create(m_mesh);
        return chunkMesh;
    }

    void ChunkMeshBuilder::addFace(const BlockFace &face,
                                   const LocalBlockPosition &position)
    {
        int index = 0;
        for (int i = 0; i < 4; i++) {
            m_mesh.vertices.push_back(face.vertices[index++] + position.x);
            m_mesh.vertices.push_back(face.vertices[index++] + position.y);
            m_mesh.vertices.push_back(face.vertices[index++] + position.z);

            m_mesh.normals.push_back(face.normal[0]);
            m_mesh.normals.push_back(face.normal[1]);
            m_mesh.normals.push_back(face.normal[2]);
        }

        m_mesh.textureCoords.insert(
            m_mesh.textureCoords.end(),
            {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f});

        m_mesh.indices.insert(m_mesh.indices.end(),
                              {m_meshMaxIndex, m_meshMaxIndex + 1,
                               m_meshMaxIndex + 2, m_meshMaxIndex + 2,
                               m_meshMaxIndex + 3, m_meshMaxIndex});
        m_meshMaxIndex += 4;
    }
} // namespace client
