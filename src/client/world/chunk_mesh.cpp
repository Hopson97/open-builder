#include "chunk_mesh.h"

#include <common/world/world_constants.h>

ChunkMesh::ChunkMesh(const ChunkPosition &chunkPosition)
    : position(chunkPosition)
{
    vertices.reserve(CHUNK_VOLUME * 2);
    textureCoords.reserve(CHUNK_VOLUME * 2);
    indices.reserve(CHUNK_VOLUME * 2);
    cardinalLights.reserve(CHUNK_VOLUME * 2);
}

void ChunkMesh::addFace(const MeshFace &face,
                        const BlockPosition &blockPosition, int texture)
{
    int index = 0;
    for (int i = 0; i < 4; i++) {
        vertices.push_back(face.vertices[index++] + position.x * CHUNK_SIZE +
                           blockPosition.x);
        vertices.push_back(face.vertices[index++] + position.y * CHUNK_SIZE +
                           blockPosition.y);
        vertices.push_back(face.vertices[index++] + position.z * CHUNK_SIZE +
                           blockPosition.z);
        cardinalLights.push_back(face.lightLevel);
    }
    textureCoords.insert(textureCoords.end(),
                         {0.0f, 0.0f, (float)texture, 1.0f, 0.0f,
                          (float)texture, 1.0f, 1.0f, (float)texture, 0.0f,
                          1.0f, (float)texture});
    indices.push_back(indicesCount);
    indices.push_back(indicesCount + 1);
    indices.push_back(indicesCount + 2);
    indices.push_back(indicesCount + 2);
    indices.push_back(indicesCount + 3);
    indices.push_back(indicesCount);
    indicesCount += 4;
}

gl::VertexArray ChunkMesh::createBuffer()
{

    gl::VertexArray vao;
    vao.bind();
    vao.addVertexBuffer(3, vertices);
    vao.addVertexBuffer(3, textureCoords);
    vao.addVertexBuffer(1, cardinalLights);
    vao.addIndexBuffer(indices);

    return vao;
}

ChunkMeshCollection::ChunkMeshCollection(const ChunkPosition &chunkPosition)
    : blockMesh(chunkPosition)
    , fluidMesh(chunkPosition)
{
}
