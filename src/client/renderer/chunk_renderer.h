#pragma once

#include "gl/shader.h"
#include "model/vertex_array.h"

#include "texture/texture2d.h"

namespace client {
    class Camera;
    class ChunkMeshObjects;

    class ChunkRenderer {
        struct ChunkShader {
            ChunkShader(const char *programName);

            void prepare(const Camera &camera);

            Shader program;
            GLuint locationCameraMatrix;
        };

      public:
        ChunkRenderer();

        void process(const ChunkMeshObjects &mesh);
        void render(const Camera &camera);

      private:
        std::vector<RenderInformation> m_solidChunkRenders;
        ChunkShader m_solidBlockShader;
        Texture2D m_texture;
    };
} // namespace client
