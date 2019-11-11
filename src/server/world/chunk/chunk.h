#pragma once

#include <array>
#include <common/constants.h>

namespace server {
    class Chunk {
      public:
        Chunk();

      private:
        std::array<CHUNK_VOLUME> m_blocks;
    }
} // namespace server
