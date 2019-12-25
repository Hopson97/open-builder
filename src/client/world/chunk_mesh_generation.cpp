#include "chunk_mesh_generation.h"

#include <SFML/System/Clock.hpp>
#include <common/world/chunk.h>
#include <iostream>

namespace {

struct MeshFace {
    std::array<GLfloat, 12> vertices;
    GLfloat lightLevel;
};

const MeshFace FRONT_FACE = {{1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1}, 0.8f};
const MeshFace LEFT_FACE = {{0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, 0.6f};
const MeshFace BACK_FACE = {{0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0}, 0.8f};
const MeshFace RIGHT_FACE = {{1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0}, 0.6f};
const MeshFace TOP_FACE = {{1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1}, 1.0f};
const MeshFace BOTTOM_FACE = {{0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1}, 0.4f};

struct ChunkMesh {
    std::vector<float> vertices;
    std::vector<GLfloat> textureCoords;
    std::vector<GLuint> indices;
    std::vector<GLfloat> cardinalLights;
    int indicesCount = 0;

    void addFace(const MeshFace &face, const ChunkPosition &chunkPosition,
                 const BlockPosition &blockPosition);
};

void ChunkMesh::addFace(const MeshFace &face,
                        const ChunkPosition &chunkPosition,
                        const BlockPosition &blockPosition)
{
    int index = 0;
    for (int i = 0; i < 4; i++) {
        vertices.push_back(face.vertices[index++] +
                           chunkPosition.x * CHUNK_SIZE + blockPosition.x);
        vertices.push_back(face.vertices[index++] +
                           chunkPosition.y * CHUNK_SIZE + blockPosition.y);
        vertices.push_back(face.vertices[index++] +
                           chunkPosition.z * CHUNK_SIZE + blockPosition.z);
        cardinalLights.push_back(face.lightLevel);
    }
    textureCoords.insert(textureCoords.end(),
                         {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f});
    indices.push_back(indicesCount);
    indices.push_back(indicesCount + 1);
    indices.push_back(indicesCount + 2);
    indices.push_back(indicesCount + 2);
    indices.push_back(indicesCount + 3);
    indices.push_back(indicesCount);
    indicesCount += 4;
}

} // namespace

gl::VertexArray makeChunkMesh(const Chunk &chunk)
{
    sf::Clock clock;
    ChunkMesh mesh;
    mesh.vertices.reserve(CHUNK_VOLUME * 2);
    mesh.textureCoords.reserve(CHUNK_VOLUME * 2);
    mesh.indices.reserve(CHUNK_VOLUME * 2);
    mesh.cardinalLights.reserve(CHUNK_VOLUME * 2);

    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                // If it is "not air"
                BlockPosition bp(x, y, z);
                if (chunk.qGetBlock(bp) == 1) {

                    // Left block face
                    if (chunk.getBlock({x - 1, y, z}) == 0) {
                        mesh.addFace(LEFT_FACE, chunk.getPosition(), bp);
                    }

                    // Right chunk face
                    if (chunk.getBlock({x + 1, y, z}) == 0) {
                        mesh.addFace(RIGHT_FACE, chunk.getPosition(), bp);
                    }

                    // Front chunk face
                    if (chunk.getBlock({x, y, z + 1}) == 0) {
                        mesh.addFace(FRONT_FACE, chunk.getPosition(), bp);
                    }

                    // Back chunk face
                    if (chunk.getBlock({x, y, z - 1}) == 0) {
                        mesh.addFace(BACK_FACE, chunk.getPosition(), bp);
                    }

                    // Bottom chunk face
                    if (chunk.getBlock({x, y - 1, z}) == 0) {
                        mesh.addFace(BOTTOM_FACE, chunk.getPosition(), bp);
                    }

                    // Top chunk face
                    if (chunk.getBlock({x, y + 1, z}) == 0) {
                        mesh.addFace(TOP_FACE, chunk.getPosition(), bp);
                    }
                }
            }
        }
    }

    gl::VertexArray vao;
    vao.create();
    vao.bind();
    vao.addVertexBuffer(3, mesh.vertices);
    vao.addVertexBuffer(2, mesh.textureCoords);
    vao.addVertexBuffer(1, mesh.cardinalLights);
    vao.addIndexBuffer(mesh.indices);

    // std::cout << clock.getElapsedTime().asMilliseconds() << std::endl;

    return vao;
}
