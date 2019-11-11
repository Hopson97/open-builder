#include "chunk_renderer.h"

#include "../world/chunk/mesh/chunk_mesh.h"
#include "camera.h"

namespace client {
    ChunkRenderer::ChunkRenderer()
    {
        m_solidBlockShader.program.load("chunk", "chunk");
        m_solidBlockShader.locationCameraMatrix =
            m_solidBlockShader.program.getUniformLocation(
                "projectionViewMatrix");
    }

    void ChunkRenderer::process(const ChunkMesh &mesh)
    {
        m_solidChunkRenders.push_back(mesh.solidBlocks.getRenderInfo());
    }

    void ChunkRenderer::render(const Camera &camera)
    {
        
    }
} // namespace client
