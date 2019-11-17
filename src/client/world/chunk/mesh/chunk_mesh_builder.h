#pragma once

#include <array>

#include "../../../renderer/model/mesh.h"
#include "../../block/block.h"
#include "chunk_mesh.h"

namespace client {
    class Chunk;

    /**
     * @brief Defines a block face: The 4 vertex positions + the normal
     * direction
     */
    struct BlockFace {
        const std::array<GLfloat, 12> vertices;
        const float basicLight;
    };

    /**
     * @brief Looks at the blocks of the chunk and builds it accordingly
     * Designed to used once and then destroyed per chunk
     */
    class ChunkMeshBuilder {
      public:
        ChunkMeshBuilder(const Chunk &chunk);

        ChunkMesh createMesh();

      private:
        void addFace(const BlockFace &face, const BlockPosition &position);

        const Chunk &mp_chunk;
        Mesh m_mesh;
        std::vector<GLfloat> m_chunkBasicLight;

        unsigned m_meshMaxIndex = 0;
    };
} // namespace client
