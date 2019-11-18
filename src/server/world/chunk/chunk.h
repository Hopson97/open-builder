#pragma once

#include "chunk_section.h"
#include <vector>
#include <SFML/System/Vector2.hpp>
#include <common/network/packet.h>

namespace server {

    class Chunk {
      public:
        Chunk(int x, int z);

        void addSection(int chunkY);

        void setBlock(int x, int y, int z, Block block);
        Block getBlock(int x, int y, int z);

        void generateTerrain();
        bool readyForSend() const;

        std::vector<Packet> createPackets() const;

      private:
        sf::Vector2i m_position;
        std::vector<ChunkSection> m_sections;
    };

} // namespace server
