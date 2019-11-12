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
        0.8f
    }};

    const client::BlockFace backFace {{
        1, 0, 0,
        0, 0, 0,
        0, 1, 0,
        1, 1, 0,
    },
    {
        0.8f
    }};

    const client::BlockFace leftFace {{
        0, 0, 0,
        0, 0, 1,
        0, 1, 1,
        0, 1, 0,
    },
    {
        0.6f
    }};

    const client::BlockFace rightFace {{
        1, 0, 1,
        1, 0, 0,
        1, 1, 0,
        1, 1, 1,
    },
    {
        0.6f
    }};

    const client::BlockFace topFace {{
        0, 1, 1,
        1, 1, 1,
        1, 1, 0,
        0, 1, 0,
    },
    {
        1.0f
    }};

    const client::BlockFace bottomFace {{
        0, 0, 0,
        1, 0, 0,
        1, 0, 1,
        0, 0, 1
    },
    {
        0.4f
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

#include <SFML/System/Clock.hpp>

namespace client {

    ChunkMeshBuilder::ChunkMeshBuilder(const Chunk &chunk)
        : mp_chunk(chunk)
    {
    }

    ChunkMesh ChunkMeshBuilder::createMesh()
    {
        ChunkMesh chunkMesh;
        sf::Clock timer;
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                for (int x = 0; x < CHUNK_SIZE; ++x) {

                    // Eventually, this will be used to determine the mesh type
                    // and texture For now, it is not needed

                    /*Block block = mp_chunk.getBlock({x, y, z});*/

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
        chunkMesh.solidBlocks.addVertexBuffer(1, m_chunkBasicLight, GL_FLOAT);
        std::cout << "Chunk creation time: "
                  << timer.getElapsedTime().asSeconds() * 1000
                  << " milliseconds" << std::endl;
        return chunkMesh;
    }

    void ChunkMeshBuilder::addFace(const BlockFace &face,
                                   const BlockPosition &position)
    {
        int index = 0;
        for (int i = 0; i < 4; i++) {
            m_mesh.vertices.push_back(face.vertices[index++] + position.x);
            m_mesh.vertices.push_back(face.vertices[index++] + position.y);
            m_mesh.vertices.push_back(face.vertices[index++] + position.z);

            m_chunkBasicLight.push_back(face.basicLight);
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
