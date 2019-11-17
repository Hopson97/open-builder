#pragma once

#include <SFML/Network/Packet.hpp>
#include <array>
#include <common/constants.h>

#include "block.h"

class Chunk {
  public:
    Chunk(int x, int y, int z);
    Chunk(const ChunkPosition &chunkPosition);

    Block getBlock(const BlockPosition &BlockPosition) const;
    void setBlock(const BlockPosition &BlockPosition, Block block);
    void quickSetBlock(const BlockPosition &BlockPosition, Block block);

    friend sf::Packet &operator<<(sf::Packet &packet, const Chunk &chunk);
    friend sf::Packet &operator>>(sf::Packet &packet, Chunk &chunk);

  protected:
    std::array<Block, CHUNK_VOLUME> m_blocks;
    const ChunkPosition position;

  private:
};
