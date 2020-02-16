#include "chunk_renderer.h"

#include "../gl/gl_errors.h"
#include <common/world/world_constants.h>
#include <numeric>

namespace {
/**
 * @brief Find the index of a chunk in a chunk renderable list given its position
 *
 * @param position The position of the chunk to find
 * @param drawables List of chunk renderable objects to search
 * @return int The index of the chunk, -1 if not found
 */
int findChunkDrawableIndex(const ChunkPosition& position,
                           const ChunkRenderList& drawables)
{
    for (unsigned i = 0; i < drawables.size(); i++) {
        if (drawables[i].position == position) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Deletes a chunk renderable
 *
 * @param position The position of the chunk renderable to delete
 * @param drawables List of chunk renderable objects to search
 */
void deleteChunkRenderable(const ChunkPosition& position, ChunkRenderList& drawables)
{
    auto index = findChunkDrawableIndex(position, drawables);
    if (index > -1) {
        drawables[index].vao.destroy();
        std::iter_swap(drawables.begin() + index, drawables.end() - 1);
        drawables.pop_back();
    }
}

/**
 * @brief Render a group of chunks
 *
 * @param chunks List of chunks to render
 * @param frustum The camera viewing frustum, used for culling chunks out of view
 * @param chunkPositionLocation Shader location to give chunk position
 * @param outResult Count of chunks rendered this frame
 */
void renderChunks(const ChunkRenderList& chunks, const ViewFrustum& frustum,
                  gl::UniformLocation chunkPositionLocation, ChunkRenderResult& outResult)
{
    for (const auto& chunk : chunks) {
        if (frustum.chunkIsInFrustum(chunk.position)) {

            glm::vec3 cp{chunk.position.x, chunk.position.y, chunk.position.z};
            cp *= CHUNK_SIZE;
            gl::loadUniform(chunkPositionLocation, cp);

            chunk.vao.getDrawable().bindAndDraw();

            outResult.chunksRendered++;
            outResult.bytesInView += chunk.bufferSize;
        }
    }
}

/**
 * @brief Buffers a list of chunk meshes
 *
 * @param mesh The vertex data to buffer
 * @param renderList The list of renderable chunks to add the buffered mesh to
 */
void bufferChunks(ChunkMesh& mesh, ChunkRenderList& renderList)
{
    if (mesh.indicesCount > 0) {
        renderList.push_back(
            {mesh.position, mesh.createBuffer(), mesh.calculateBufferSize()});
    }
}

} // namespace

void ChunkRenderer::init()
{ // Chunk shader
    m_solidShader.program.create("chunk/chunk", "chunk/chunk");
    m_solidShader.program.bind();
    m_solidShader.projectionViewLocation =
        m_solidShader.program.getUniformLocation("projectionViewMatrix");
    m_solidShader.chunkPositionLocation =
        m_solidShader.program.getUniformLocation("chunkPosition");

    // Fluids shader
    m_fluidShader.program.create("chunk/water", "chunk/chunk");
    m_fluidShader.program.bind();
    m_fluidShader.projectionViewLocation =
        m_fluidShader.program.getUniformLocation("projectionViewMatrix");
    m_fluidShader.timeLocation = m_fluidShader.program.getUniformLocation("time");
    m_fluidShader.chunkPositionLocation =
        m_fluidShader.program.getUniformLocation("chunkPosition");

    // Flora shader
    m_floraShader.program.create("chunk/flora", "chunk/transparent");
    m_floraShader.program.bind();
    m_floraShader.projectionViewLocation =
        m_floraShader.program.getUniformLocation("projectionViewMatrix");
    m_floraShader.timeLocation = m_floraShader.program.getUniformLocation("time");
    m_floraShader.chunkPositionLocation =
        m_floraShader.program.getUniformLocation("chunkPosition");
}

void ChunkRenderer::updateMesh(const ChunkPosition& position,
                               ChunkMeshCollection&& meshes)
{
    m_chunkMeshes.push_back(std::move(meshes));
    deleteChunkRenderables(position);
}

ChunkRenderResult ChunkRenderer::renderChunks(const glm::vec3& cameraPosition,
                                              const ViewFrustum& frustum,
                                              const glm::mat4& projectionViewMatrix,
                                              bool cameraInWater)
{
    auto& solidDrawables = m_chunkRenderables[static_cast<int>(ChunkRenderType::Solid)];
    auto& fluidDrawables = m_chunkRenderables[static_cast<int>(ChunkRenderType::Fluid)];
    auto& floraDrawables = m_chunkRenderables[static_cast<int>(ChunkRenderType::Flora)];

    for (auto& meshes : m_chunkMeshes) {
        bufferChunks(meshes.voxelMesh, solidDrawables);
        bufferChunks(meshes.fluidMesh, fluidDrawables);
        bufferChunks(meshes.floraMesh, floraDrawables);
    }
    m_chunkMeshes.clear();

    float time = m_animationTimer.getElapsedTime().asSeconds();
    ChunkRenderResult result;
    // Solid voxels
    m_solidShader.program.bind();
    gl::loadUniform(m_solidShader.projectionViewLocation, projectionViewMatrix);
    ::renderChunks(solidDrawables, frustum, m_solidShader.chunkPositionLocation, result);

    // Fluid voxels
    m_fluidShader.program.bind();
    gl::loadUniform(m_fluidShader.projectionViewLocation, projectionViewMatrix);
    gl::loadUniform(m_fluidShader.timeLocation, time);
    glCheck(glEnable(GL_BLEND));
    if (cameraInWater) {
        glCheck(glCullFace(GL_FRONT));
    }
    ::renderChunks(fluidDrawables, frustum, m_fluidShader.chunkPositionLocation, result);
    glCheck(glCullFace(GL_BACK));
    glCheck(glDisable(GL_BLEND));

    // Flora voxels
    m_floraShader.program.bind();
    gl::loadUniform(m_floraShader.projectionViewLocation, projectionViewMatrix);
    gl::loadUniform(m_floraShader.timeLocation, time);
    glDisable(GL_CULL_FACE);
    ::renderChunks(floraDrawables, frustum, m_floraShader.chunkPositionLocation, result);
    glEnable(GL_CULL_FACE);

    return result;
}

int ChunkRenderer::getTotalChunks() const
{
    auto addSize = [](int sum, const ChunkRenderList& chunks) {
        return sum + chunks.size();
    };

    return std::accumulate(m_chunkRenderables.begin(), m_chunkRenderables.end(), 0,
                           addSize);
}

int ChunkRenderer::getTotalBufferSize() const
{
    auto addSize = [](int sum, const ChunkRenderList& chunks) {
        size_t totalSize = 0;
        for (auto& chunk : chunks) {
            totalSize += chunk.bufferSize;
        }
        return sum + totalSize;
    };

    return std::accumulate(m_chunkRenderables.begin(), m_chunkRenderables.end(), 0,
                           addSize);
}

void ChunkRenderer::deleteChunkRenderables(const ChunkPosition& position)
{
    for (auto& chunkRenderList : m_chunkRenderables) {
        deleteChunkRenderable(position, chunkRenderList);
    }
}
