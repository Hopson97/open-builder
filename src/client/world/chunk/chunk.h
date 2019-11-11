#pragma once

#include "../block/block.h"
#include <array>

namespace client {
    class Chunk {
      public:
        constexpr static int SIZE = 32;

      public:
        Chunk();

        Block getBlock(const LocalBlockPosition &position) const;

      private:
        std::array<Block, SIZE * SIZE * SIZE> m_blocks;
    };
} // namespace client
