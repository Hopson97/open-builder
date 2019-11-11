#pragma once

#include <array>
#include <common/constants.h>

#include "../block/block.h"

namespace server {
    class Chunk {
      public:
        Chunk();

        Block getBlock(const BlockPosition &position) const;
        void setBlock(const BlockPosition &position, Block block);
        void quickSetBlock(const BlockPosition &position, Block block);

      private:
        std::array<Block, CHUNK_VOLUME> m_blocks;
    };
} // namespace server
