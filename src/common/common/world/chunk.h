#pragma once

#include <SFML/Network/Packet.hpp>
#include <array>
#include <common/constants.h>

#include "block.h"

class Chunk {
  public:
    Chunk(int x, int y, int z);
    Chunk(const ChunkPosition &chunkPosition);

    Block getBlock(const BlockPosition &blockPosition) const;
    void setBlock(const BlockPosition &blockPosition, Block block);
    void quickSetBlock(const BlockPosition &blockPosition, Block block);

    friend sf::Packet &operator<<(sf::Packet &packet, const Chunk &chunk);
    friend sf::Packet &operator>>(sf::Packet &packet, Chunk &chunk);

  private:
    std::array<Block, CHUNK_VOLUME> m_blocks;

  public:
    const ChunkPosition position;
};
