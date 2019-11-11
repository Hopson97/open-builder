#pragma once

#include <array>
#include <common/constants.h>
#include <SFML/Network/Packet.hpp>

#include "../block/block.h"

namespace server {
    class Chunk {
      public:
        Chunk(int x, int y, int z);
        Chunk(const ChunkPosition &chunkPosition);

        Block getBlock(const BlockPosition &BlockPosition) const;
        void setBlock(const BlockPosition &BlockPosition, Block block);
        void quickSetBlock(const BlockPosition &BlockPosition, Block block);

        friend sf::Packet& operator<<(sf::Packet& packet, const Chunk& chunk);

      private:
        std::array<Block, CHUNK_VOLUME> m_blocks;

      public:
        const ChunkPosition position;

      private:
    };

    
} // namespace server
