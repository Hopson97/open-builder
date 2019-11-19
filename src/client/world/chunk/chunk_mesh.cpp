#include "chunk_mesh.h"

#include "chunk.h"
#include <array>
#include <iostream>

#include <SFML/System/Clock.hpp>

namespace {
    /**
     * @brief Defines a block face: The 4 vertex positions + the normal
     * direction
     */
    struct BlockFace {
        const std::array<GLfloat, 12> vertices;
        const float basicLight;
    };
    // clang-format off
    const BlockFace frontFace {{
        0, 0, 1,
        1, 0, 1,
        1, 1, 1,
        0, 1, 1,
    },
        0.8f
    };

    const BlockFace backFace {{
        1, 0, 0,
        0, 0, 0,
        0, 1, 0,
        1, 1, 0,
    },
        0.8f
    };

    const BlockFace leftFace {{
        0, 0, 0,
        0, 0, 1,
        0, 1, 1,
        0, 1, 0,
    },
    
        0.6f
    };

    const BlockFace rightFace {{
        1, 0, 1,
        1, 0, 0,
        1, 1, 0,
        1, 1, 1,
    },
        0.6f
    };

    const BlockFace topFace {{
        0, 1, 1,
        1, 1, 1,
        1, 1, 0,
        0, 1, 0,
    },
        1.0f
    };

    const BlockFace bottomFace {{
        0, 0, 0,
        1, 0, 0,
        1, 0, 1,
        0, 0, 1
    },
        0.4f
    };

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
    void addFace(const ChunkSectionPosition &chunkPosition,
                 const BlockPosition &blockPosition, const BlockFace &face,
                 client::ChunkMesh &mesh)
    {
        int index = 0;
        for (int i = 0; i < 4; i++) {
            mesh.basicInfo.vertices.push_back(face.vertices[index++] +
                                              blockPosition.x +
                                              (chunkPosition.x * CHUNK_SIZE));
            mesh.basicInfo.vertices.push_back(face.vertices[index++] +
                                              blockPosition.y +
                                              (chunkPosition.y * CHUNK_SIZE));
            mesh.basicInfo.vertices.push_back(face.vertices[index++] +
                                              blockPosition.z +
                                              (chunkPosition.z * CHUNK_SIZE));

            mesh.basicLight.push_back(face.basicLight);
        }

        mesh.basicInfo.textureCoords.insert(
            mesh.basicInfo.textureCoords.end(),
            {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f});

        mesh.basicInfo.indices.insert(mesh.basicInfo.indices.end(),
                                      {mesh.maxIndex, mesh.maxIndex + 1,
                                       mesh.maxIndex + 2, mesh.maxIndex + 2,
                                       mesh.maxIndex + 3, mesh.maxIndex});
        mesh.maxIndex += 4;
    }
} // namespace

namespace client {
    ChunkMeshGroup createChunkMesh(const ChunkSection &section)
    {
        static float total = 0;
        static int count = 0;
        sf::Clock timer;

        ChunkMeshGroup group;
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                for (int x = 0; x < CHUNK_SIZE; ++x) {
                    Block block = section.getBlock({x, y, z});
                    if (block.type == BlockType::Grass) {

                        Block up = section.getBlock({x, y + 1, z});
                        Block down = section.getBlock({x, y - 1, z});

                        Block left = section.getBlock({x - 1, y, z});
                        Block right = section.getBlock({x + 1, y, z});

                        Block forwards = section.getBlock({x, y, z + 1});
                        Block back = section.getBlock({x, y, z - 1});

                        if (up.type == BlockType::Air) {
                            addFace(section.getPosition(), {x, y, z}, topFace,
                                    group.solidMesh);
                        }
                        if (down.type == BlockType::Air) {
                            addFace(section.getPosition(), {x, y, z},
                                    bottomFace, group.solidMesh);
                        }

                        if (left.type == BlockType::Air) {
                            addFace(section.getPosition(), {x, y, z}, leftFace,
                                    group.solidMesh);
                        }
                        if (right.type == BlockType::Air) {
                            addFace(section.getPosition(), {x, y, z}, rightFace,
                                    group.solidMesh);
                        }
                        if (forwards.type == BlockType::Air) {
                            addFace(section.getPosition(), {x, y, z}, frontFace,
                                    group.solidMesh);
                        }
                        if (back.type == BlockType::Air) {
                            addFace(section.getPosition(), {x, y, z}, backFace,
                                    group.solidMesh);
                        }
                    }
                }
            }
        }

        float time = timer.getElapsedTime().asSeconds();
        total += time;
        count++;
        std::cout << time * 1000 << " " << (total / count) * 1000 << '\n';

        return group;
    }

    ChunkMeshObjects bufferChunkMeshGroup(ChunkMeshGroup &group)
    {
        ChunkMeshObjects objects;
        objects.solid.create(group.solidMesh.basicInfo);
        objects.solid.addVertexBuffer(1, group.solidMesh.basicLight, GL_FLOAT);
        return objects;
    }
} // namespace client
