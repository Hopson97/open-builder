#pragma once

#include "gl/shader.h"
#include "model/vertex_array.h"

namespace client {
    class Camera;
    class ChunkMesh;

    class ChunkRenderer
    {
        public:
            ChunkRenderer();

            void process(const ChunkMesh& mesh);

        private:
            std::vector<ChunkMesh*> solid;
    };
} // namespace client
