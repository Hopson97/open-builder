#pragma once

#include "../block/block.h"
#include <array>
#include <iostream>
#include <common/constants.h>
#include <SFML/Network/Packet.hpp>

namespace client {
    class Chunk {
      public:        
        enum class Flag : u8
        {
          None = 0,
          NeedsNewMesh = 1,
        };
        
        Chunk(int x, int y, int z);
        Chunk(const ChunkPosition &chunkPosition);

        Block getBlock(const BlockPosition &blockPosition) const;

        friend sf::Packet& operator>>(sf::Packet& packet, Chunk& chunk);

      private:
        std::array<Block, CHUNK_VOLUME> m_blocks;

      public:
        const ChunkPosition position;
        Flag flag = Flag::None;
    };
} // namespace client
