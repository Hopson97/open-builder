#pragma once

#include "../block/block.h"
#include <SFML/Network/Packet.hpp>
#include <array>
#include <common/constants.h>
#include <iostream>

namespace client {
    class ChunkSection {
      public:
        enum class Flag : u8 {
            None = 0,
            NeedsNewMesh = 1,
        };

        ChunkSection(int x, int y, int z);
        ChunkSection(const ChunkPosition &chunkPosition);

        Block getBlock(const BlockPosition &blockPosition) const;

        friend sf::Packet &operator>>(sf::Packet &packet, ChunkSection &chunk);

      private:
        std::array<Block, CHUNK_VOLUME> m_blocks;

      public:
        ChunkPosition position;
        Flag flag = Flag::None;
    };
} // namespace client
