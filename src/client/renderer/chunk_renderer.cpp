#include "chunk_renderer.h"

#include "../world/chunk/mesh/chunk_mesh.h"
#include "camera.h"

#include <iostream>

namespace client {
    ChunkRenderer::ChunkRenderer()
        : m_solidBlockShader("chunk")
    {
    }

    void ChunkRenderer::process(const ChunkMesh &mesh)
    {
        m_solidChunkRenders.push_back(mesh.solidBlocks.getRenderInfo());
    }

    void ChunkRenderer::render(const Camera &camera)
    {
        // Solid blocks first
        m_solidBlockShader.prepare(camera);
        for (auto &chunk : m_solidChunkRenders) {
            chunk.bindAndDraw();
        }

        m_solidChunkRenders.clear();
    }

    ChunkRenderer::ChunkShader::ChunkShader(const char *programName)
        : program(programName, programName)
    {
        locationCameraMatrix =
            program.getUniformLocation("projectionViewMatrix");
    }

    void ChunkRenderer::ChunkShader::prepare(const Camera &camera)
    {
        program.use();
        program.loadMatrix4(locationCameraMatrix,
                            camera.getProjectionViewMatrix());
    }
} // namespace client
