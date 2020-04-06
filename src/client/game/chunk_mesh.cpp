#include "chunk_mesh.h"

#include <common/world/world_constants.h>
namespace {
    template <typename T>
    size_t vecSize(const std::vector<T> vect)
    {
        return vect.size() * sizeof(vect[0]);
    }
} // namespace

ChunkMesh::ChunkMesh(const ChunkPosition& chunkPosition)
    : position(chunkPosition)
{
    vertexData.reserve(CHUNK_VOLUME * 2);
    indices.reserve(CHUNK_VOLUME * 2);
}

void ChunkMesh::addFace(const MeshFace& face, const VoxelPosition& voxelPosition,
                        GLuint texture)
{
    int index = 0;
    for (unsigned i = 0; i < 4; i++) {
        GLubyte x = face.vertices[index++] + voxelPosition.x;
        GLubyte y = face.vertices[index++] + voxelPosition.y;
        GLubyte z = face.vertices[index++] + voxelPosition.z;

        // Packs the vertex coordinates, cardinal light, and texture coordinates into 4
        // bytes
        GLuint vertex =
            x | y << 6 | z << 12 | face.lightLevel << 18 | i << 21 | texture << 23;

        vertexData.push_back(vertex);
    }
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
    vao.addVertexBuffer(1, vertexData);
    vao.addIndexBuffer(indices);
    return vao;
}

size_t ChunkMesh::calculateBufferSize() const
{
    return vecSize(vertexData) + vecSize(indices);
}

ChunkMeshCollection::ChunkMeshCollection(const ChunkPosition& chunkPosition)
    : voxelMesh(chunkPosition)
    , fluidMesh(chunkPosition)
    , floraMesh(chunkPosition)
{
}
