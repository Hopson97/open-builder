#pragma once

#include <array>
#include <common/constants.h>

#include "../block/block.h"

namespace server {
    class Chunk {
      public:
        Chunk(int x, int y, int z);
        Chunk(const ChunkPosition &chunkPosition);

        Block getBlock(const BlockPosition &BlockPosition) const;
        void setBlock(const BlockPosition &BlockPosition, Block block);
        void quickSetBlock(const BlockPosition &BlockPosition, Block block);

      private:
        std::array<Block, CHUNK_VOLUME> m_blocks;

      public:
        const ChunkPosition position;

      private:
    };
} // namespace server
