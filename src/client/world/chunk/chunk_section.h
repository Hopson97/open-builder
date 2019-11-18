#pragma once

#include "../block/block.h"
#include <SFML/Network/Packet.hpp>
#include <array>
#include <common/constants.h>
#include <iostream>

namespace client {
  class World;
    class ChunkSection {
      public:
        ChunkSection(int x, int y, int z, World& world);

        Block getBlock(const BlockPosition &blockPosition) const;

        const ChunkPosition& getPosition() const;

        friend sf::Packet &operator>>(sf::Packet &packet, ChunkSection &chunk);

      private:
        std::array<Block, CHUNK_VOLUME> m_blocks;
        World* mp_world;
        ChunkPosition m_position;

    };
} // namespace client
