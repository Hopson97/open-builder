#include "chunk_mesh_generation.h"

#include <common/world/chunk.h>

namespace {
struct ChunkMesh {
    std::vector<float> verticies;
    std::vector<GLfloat> texCoords;
    std::vector<GLuint> indices;
};

using face = std::array<GLfloat, 12>;

const face FONT_FACE = {1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1};
const face LEFT_FACE = {0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1};
const face BACK_FACE = {0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0};
const face RIGHT_FACE = {1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0};
const face TOP_FACE = {1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1};
const face BOTTOM_FACE = {0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1};
} // namespace

gl::VertexArray makeChunkMesh(const Chunk &chunk)
{

}
