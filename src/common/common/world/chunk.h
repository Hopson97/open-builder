#pragma once

#include "../types.h"
#include "../constants.h"
#include "block.h"

class ChunkTEMP {
  public:
    ChunkTEMP(const ChunkPosition &chunkPosition);
    virtual ~ChunkTEMP() = default;

    Block getBlock(const BlockPosition &blockPosition) const;

    void setBlock(const BlockPosition &blockPosition, Block block);
    void quickSetBlock(const BlockPosition &BlockPosition, Block block);

  public:
    const ChunkPosition position;

  protected:
    std::array<Block, CHUNK_VOLUME> m_blocks;
};