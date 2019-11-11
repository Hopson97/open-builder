#pragma once

#include "gl/shader.h"
#include "model/vertex_array.h"

namespace client {
    class Camera;
    class ChunkMesh;

    class ChunkRenderer {
      public:
        ChunkRenderer();

        void process(const ChunkMesh &mesh);
        void render(const Camera &camera);

      private:
        std::vector<RenderInformation> m_solidChunkRenders;

        struct {
            Shader program;
            GLuint locationCameraMatrix;
        } m_solidBlockShader;
    };
} // namespace client
