#pragma once

#include "world_constants.h"
#include <array>
#include "../types.h"
#include "coordinate.h"

class ChunkManager;

struct Chunk {
  public:
    Chunk(ChunkManager *manager);

    u8 qGetBlock(const BlockPosition &blockPosition) const;
    void qSetBlock(const BlockPosition &blockPosition, u8 block);

  private:
    std::array<u8, CHUNK_VOLUME> blocks{0};
    ChunkManager *mp_manager;

};

class ChunkManager final {
  public:
    Chunk& addChunk(const ChunkPosition &chunk);

    u8 getBlock(const BlockPosition &blockPosition) const;
    void setBlock(const BlockPosition &blockPosition, u8 block);

    bool hasChunk(const ChunkPosition &chunk) const;
    bool hasNeighbours(const ChunkPosition &chunkPosition) const;

  private:
    ChunkPositionMap<Chunk> m_chunks;
};