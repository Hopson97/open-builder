#pragma once

#include "../block/block.h"
#include <array>
#include <common/constants.h>

namespace client {
    class Chunk {
      public:
        Chunk();

        Block getBlock(const LocalBlockPosition &position) const;

      private:
        std::array<Block, CHUNK_VOLUME> m_blocks;
    };
} // namespace client
