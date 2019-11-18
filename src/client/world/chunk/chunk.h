#pragma once

#include "chunk_section.h"
#include <SFML/System/Vector2.hpp>

namespace client {

    class Chunk {
      public:
        Chunk(int x, int z);

        void addSection(int chunkY);
        Block getBlock(int x, int y, int z);

      private:
        sf::Vector2i m_position;
        std::vector<ChunkSection> m_sections;
    };
} // namespace client
