#pragma once

#include <SFML/Network/Packet.hpp>
#include <array>
#include <common/constants.h>

#include "../block/block.h"

namespace server {
    class ChunkSection final {
      public:
        ChunkSection(const ChunkSectionPosition &chunkPosition);

        Block getBlock(const BlockPosition &BlockPosition) const;
        void setBlock(const BlockPosition &BlockPosition, Block block);
        void quickSetBlock(const BlockPosition &BlockPosition, Block block);

        const ChunkSectionPosition &getPosition() const;

        friend sf::Packet &operator<<(sf::Packet &packet,
                                      const ChunkSection &chunk);

      private:
        std::array<Block, CHUNK_VOLUME> m_blocks;
        ChunkSectionPosition m_position;
    };

} // namespace server
