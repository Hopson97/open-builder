#pragma once

#include <SFML/Network/Packet.hpp>
#include <array>
#include <common/constants.h>

#include "../block/block.h"

namespace server {
    class ChunkSection {
      public:
        ChunkSection(int x, int y, int z);
        ChunkSection(const ChunkPosition &chunkPosition);

        Block getBlock(const BlockPosition &BlockPosition) const;
        void setBlock(const BlockPosition &BlockPosition, Block block);
        void quickSetBlock(const BlockPosition &BlockPosition, Block block);

        friend sf::Packet &operator<<(sf::Packet &packet, const ChunkSection &chunk);

      private:
        std::array<Block, CHUNK_VOLUME> m_blocks;

      public:
        const ChunkPosition position;

      private:
    };

} // namespace server
