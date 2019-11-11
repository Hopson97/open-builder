#pragma once

#include "../../renderer/model/vertex_array.h"

namespace client {
    class Chunk;

    /**
     * @brief Drawable representation of a chunk
     * 
     */
    class ChunkMesh {
        public:

        private:
            VertexArray array;
    };

    /**
     * @brief Looks at the blocks of the chunk and builds it accordingly
     * Designed to used once and then destroyed per chunk
     */
    class ChunkMeshBuilder {
        public:
            ChunkMeshBuilder(const Chunk& chunk);

            ChunkMesh createMesh();

        private:
            const Chunk& mp_chunk;
    };
} // namespace client
