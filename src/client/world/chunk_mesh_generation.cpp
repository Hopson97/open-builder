#include "chunk_mesh_generation.h"

#include <common/world/chunk.h>
#include <iostream>

namespace {
using Face = std::array<GLfloat, 12>;
      
const Face FRONT_FACE = {1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1};
const Face LEFT_FACE = {0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1};
const Face BACK_FACE = {0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0};
const Face RIGHT_FACE = {1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0};
const Face TOP_FACE = {1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1};
const Face BOTTOM_FACE = {0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1};

struct ChunkMesh {
    std::vector<float> verticies;
    std::vector<GLfloat> textureCoords;
    std::vector<GLuint> indices;
    int indicesCount = 0;
                   
    void addFace(const Face &face, const ChunkPosition& chunkPosition, const BlockPosition& blockPosition)
    {
        int index = 0;
        for (int i = 0; i < 4; i++) {
            verticies.push_back(face[index++] + chunkPosition.x * CHUNK_SIZE + blockPosition.x); 
            verticies.push_back(face[index++] + chunkPosition.y * CHUNK_SIZE +
                                blockPosition.y);
            verticies.push_back(face[index++ ] + chunkPosition.z * CHUNK_SIZE +
                                blockPosition.z);
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
};

} // namespace


gl::VertexArray makeChunkMesh(const Chunk &chunk)
{
    ChunkMesh mesh;

    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                //If it is "not air"
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
    vao.addVertexBuffer(3, mesh.verticies);
    vao.addVertexBuffer(2, mesh.textureCoords);
    vao.addIndexBuffer(mesh.indices);

    return vao;
}
