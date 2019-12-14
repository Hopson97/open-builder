#pragma once

#include "world_constants.h"
#include <array>
#include "../types.h"
#include "coordinate.h"

struct Chunk {
    std::array<u8, CHUNK_VOLUME> blocks{0};

    u8 getBlock(const BlockPosition &blockPosition) const;
    void setBlock(const BlockPosition &blockPosition, u8 block);
};

class ChunkManager {
  public:
    Chunk& addChunk(const ChunkPosition &chunk);

    u8 getBlock(const BlockPosition &blockPosition) const;
    void setBlock(const BlockPosition &blockPosition, u8 block);

  private:
    ChunkPositionMap<Chunk> m_chunks;
};