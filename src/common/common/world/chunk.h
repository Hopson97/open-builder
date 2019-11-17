#pragma once

#include "../constants.h"
#include "../types.h"
#include "block.h"

class Chunk {
  public:
    Chunk(const ChunkPosition &chunkPosition);
    virtual ~Chunk() = default;

    Block getBlock(const BlockPosition &blockPosition) const;

    void setBlock(const BlockPosition &blockPosition, Block block);
    void quickSetBlock(const BlockPosition &BlockPosition, Block block);

  public:
    const ChunkPosition position;

  protected:
    std::array<Block, CHUNK_VOLUME> m_blocks;
};