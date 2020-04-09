#include "chunk_renderer.h"

#include "../client_config.h"
#include "../gl/gl_errors.h"
#include "camera.h"
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
     * @brief Checks whether chunk is within the render distance using Euclidean distance
     *
     * @param cameraPosition The position of the camera (in world coordinates)
     * @param chunkPosition The position of the chunk (in "chunk" coordinates)
     * @param renderDistance The render distance
     */
    bool isChunkInRenderDistance(const glm::vec3& cameraPosition,
                                 const ChunkPosition& chunkPosition)
    {
        float renderDistance = ClientConfig::get().renderDistance;
        return std::pow((int)(cameraPosition.x / CHUNK_SIZE) - chunkPosition.x, 2) +
                   std::pow((int)(cameraPosition.y / CHUNK_SIZE) - chunkPosition.y, 2) +
                   std::pow((int)(cameraPosition.z / CHUNK_SIZE) - chunkPosition.z, 2) <=
               std::pow(renderDistance, 2);
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
                      gl::UniformLocation chunkPositionLocation,
                      ChunkRenderResult& outResult, const glm::vec3& cameraPosition)
    {
        for (const auto& chunk : chunks) {
            if (isChunkInRenderDistance(cameraPosition, chunk.position) &&
                frustum.chunkIsInFrustum(chunk.position)) {

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
    m_shader.program.create("chunk", "chunk");
    m_shader.program.bind();
    m_shader.projectionViewLocation =
        m_shader.program.getUniformLocation("projectionViewMatrix");
    m_shader.chunkPositionLocation = m_shader.program.getUniformLocation("chunkPosition");
}

void ChunkRenderer::updateMesh(const ChunkPosition& position,
                               ChunkMeshCollection&& meshes)
{
    m_chunkMeshes.push_back(std::move(meshes));
    deleteChunkRenderables(position);
}

ChunkRenderResult ChunkRenderer::renderChunks(const Camera& camera, bool cameraInWater)
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

    const glm::mat4& pv = camera.getProjectionView();
    const glm::vec3& pos = camera.getPosition();
    const ViewFrustum& frustum = camera.getFrustum();

    ChunkRenderResult result;
    // Solid voxels
    m_shader.program.bind();
    gl::loadUniform(m_shader.projectionViewLocation, pv);
    ::renderChunks(solidDrawables, frustum, m_shader.chunkPositionLocation, result, pos);

    // Flora voxels
    glDisable(GL_CULL_FACE);
    ::renderChunks(floraDrawables, frustum, m_shader.chunkPositionLocation, result, pos);
    glEnable(GL_CULL_FACE);

    // Fluid voxels
    glCheck(glEnable(GL_BLEND));
    if (cameraInWater) {
        glCheck(glCullFace(GL_FRONT));
    }
    ::renderChunks(fluidDrawables, frustum, m_shader.chunkPositionLocation, result, pos);
    glCheck(glCullFace(GL_BACK));
    glCheck(glDisable(GL_BLEND));

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
