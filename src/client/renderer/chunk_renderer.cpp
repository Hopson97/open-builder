#include "chunk_renderer.h"

#include <common/world/world_constants.h>

#include "../gl/gl_errors.h"

namespace {
int findChunkDrawableIndex(const ChunkPosition& position,
                           const std::vector<ChunkRenderable>& drawables)
{
    for (int i = 0; i < static_cast<int>(drawables.size()); i++) {
        if (drawables[i].position == position) {
            return i;
        }
    }
    return -1;
}
void deleteChunkRenderable(const ChunkPosition& position,
                           std::vector<ChunkRenderable>& drawables)
{
    auto index = findChunkDrawableIndex(position, drawables);
    if (index > -1) {
        drawables[index].vao.destroy();

        // As the chunk renders need not be a sorted array, "swap and pop"
        // can be used
        // More efficent (and maybe safer) than normal deletion
        std::iter_swap(drawables.begin() + index, drawables.end() - 1);
        drawables.pop_back();
    }
}

int renderChunks(const std::vector<ChunkRenderable>& chunks, const ViewFrustum& frustum,
                 gl::UniformLocation chunkPositionLocation, size_t& bytes)
{
    int renderedChunks = 0;
    for (const auto& chunk : chunks) {
        if (frustum.chunkIsInFrustum(chunk.position)) {
            glm::vec3 cp{chunk.position.x, chunk.position.y, chunk.position.z};
            cp *= CHUNK_SIZE;
            gl::loadUniform(chunkPositionLocation, cp);

            chunk.vao.getDrawable().bindAndDraw();
            renderedChunks++;
            bytes += chunk.bufferSize;
        }
    }
    return renderedChunks;
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

void ChunkRenderer::renderChunks(const glm::vec3& cameraPosition,
                                 const ViewFrustum& frustum,
                                 const glm::mat4& projectionViewMatrix)
{
    for (auto& meshes : m_chunkMeshes) {
        // TODO [Hopson] -> DRY this code somehow...
        if (meshes.blockMesh.indicesCount > 0) {
            m_solidDrawables.push_back({meshes.blockMesh.position,
                                        meshes.blockMesh.createBuffer(),
                                        meshes.blockMesh.calculateBufferSize()});
        }
        
        if (meshes.fluidMesh.indicesCount > 0) {
            m_fluidDrawables.push_back(
                {meshes.fluidMesh.position, meshes.fluidMesh.createBuffer(), meshes.fluidMesh.calculateBufferSize()});
        }
        /**
        if (chunkMesh.floraMesh.indicesCount > 0) {
            m_chunks.floraDrawables.push_back(
                {chunkMesh.floraMesh.position, chunkMesh.floraMesh.createBuffer()});
        }
        */
    }
    m_chunkMeshes.clear();

    unsigned temp = 0;
    float time = m_animationTimer.getElapsedTime().asSeconds();

    // Solid blocks
    m_solidShader.program.bind();
    gl::loadUniform(m_solidShader.projectionViewLocation, projectionViewMatrix);
    ::renderChunks(m_solidDrawables, frustum, m_solidShader.chunkPositionLocation, temp);

    // Fluid blocks
    m_fluidShader.program.bind();
    gl::loadUniform(m_fluidShader.projectionViewLocation, projectionViewMatrix);
    gl::loadUniform(m_fluidShader.timeLocation, time);

    glCheck(glEnable(GL_BLEND));
    ::renderChunks(m_fluidDrawables, frustum, m_fluidShader.chunkPositionLocation,
                 temp);
    glCheck(glDisable(GL_BLEND));

    //TODO Player is in water and all that idk
    // if in water blah blah
    // glCheck(glCullFace(GL_FRONT));
    //  here
    // glCheck(glCullFace(GL_BACK));



}

void ChunkRenderer::deleteChunkRenderables(const ChunkPosition& position)
{
    deleteChunkRenderable(position, m_solidDrawables);
    deleteChunkRenderable(position, m_fluidDrawables);
    deleteChunkRenderable(position, m_floraDrawables);
}
