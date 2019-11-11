#pragma once

#include "../block/block.h"
#include <array>

namespace client {
    class Chunk {
      public:
        Chunk();

        Block getBlock(const LocalBlockPosition &position) const;

      private:
        std::array<Block, 32 * 32 * 32> m_blocks;
    };
} // namespace client
