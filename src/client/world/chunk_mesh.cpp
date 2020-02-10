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
}

void ChunkMesh::addFace(const MeshFace& face, const BlockPosition& blockPosition,
                        GLuint texture)
{
    int index = 0;
    std::vector<GLuint> quad;
    quad.reserve(4);
    for (unsigned i = 0; i < 4; i++) {
        GLubyte x = face.vertices[index++] + blockPosition.x;
        GLubyte y = face.vertices[index++] + blockPosition.y;
        GLubyte z = face.vertices[index++] + blockPosition.z;

        // Packs the vertex coordinates, cardinal light, and texture coordinates into 4
        // bytes
        GLuint vertex =
            x | y << 6 | z << 12 | face.lightLevel << 18 | i << 21 | texture << 23;

        quad.push_back(vertex);
    }
    vertexData.push_back(quad[0]);
    vertexData.push_back(quad[1]);
    vertexData.push_back(quad[2]);
    vertexData.push_back(quad[2]);
    vertexData.push_back(quad[3]);
    vertexData.push_back(quad[0]);
}

gl::VertexArray ChunkMesh::createBuffer()
{
    gl::VertexArray vao;
    vao.bind();
    vao.setIndicesCount(vertexData.size());
    vao.addVertexBuffer(1, vertexData);
    return vao;
}

size_t ChunkMesh::calculateBufferSize() const
{
    return vecSize(vertexData);
}

size_t ChunkMesh::vertexCount() const
{
    return vertexData.size();
}

ChunkMeshCollection::ChunkMeshCollection(const ChunkPosition& chunkPosition)
    : blockMesh(chunkPosition)
    , fluidMesh(chunkPosition)
    , floraMesh(chunkPosition)
{
}
