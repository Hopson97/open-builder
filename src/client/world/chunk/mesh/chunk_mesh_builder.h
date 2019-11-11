#pragma once

#include "chunk_mesh.h"

namespace client
{
    class Chunk;
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
} // namespace clinet
