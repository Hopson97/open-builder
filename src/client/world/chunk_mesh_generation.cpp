#include "chunk_mesh_generation.h"

#include <SFML/System/Clock.hpp>
#include <common/world/chunk.h>
#include <iostream>

namespace {

const MeshFace FRONT_FACE = {{1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1}, 0.8f};
const MeshFace LEFT_FACE = {{0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, 0.6f};
const MeshFace BACK_FACE = {{0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0}, 0.8f};
const MeshFace RIGHT_FACE = {{1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0}, 0.6f};
const MeshFace TOP_FACE = {{1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1}, 1.0f};
const MeshFace BOTTOM_FACE = {{0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1}, 0.4f};

} // namespace

ChunkMesh makeChunkMesh(const Chunk &chunk)
{
    sf::Clock clock;
    ChunkMesh mesh;
    mesh.position = chunk.getPosition();
    mesh.vertices.reserve(CHUNK_VOLUME * 2);
    mesh.textureCoords.reserve(CHUNK_VOLUME * 2);
    mesh.indices.reserve(CHUNK_VOLUME * 2);
    mesh.cardinalLights.reserve(CHUNK_VOLUME * 2);

    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                // If it is "not air"
                BlockPosition blockPosition(x, y, z);
                if (chunk.qGetBlock(blockPosition) == 1) {

                    // Left block face
                    if (chunk.getBlock({x - 1, y, z}) == 0) {
                        mesh.addFace(LEFT_FACE, blockPosition);
                    }

                    // Right chunk face
                    if (chunk.getBlock({x + 1, y, z}) == 0) {
                        mesh.addFace(RIGHT_FACE, blockPosition);
                    }

                    // Front chunk face
                    if (chunk.getBlock({x, y, z + 1}) == 0) {
                        mesh.addFace(FRONT_FACE, blockPosition);
                    }

                    // Back chunk face
                    if (chunk.getBlock({x, y, z - 1}) == 0) {
                        mesh.addFace(BACK_FACE, blockPosition);
                    }

                    // Bottom chunk face
                    if (chunk.getBlock({x, y - 1, z}) == 0) {
                        mesh.addFace(BOTTOM_FACE, blockPosition);
                    }

                    // Top chunk face
                    if (chunk.getBlock({x, y + 1, z}) == 0) {
                        mesh.addFace(TOP_FACE, blockPosition);
                    }
                }
            }
        }
    }
    // std::cout << clock.getElapsedTime().asMilliseconds() << std::endl;

    return mesh;
}
