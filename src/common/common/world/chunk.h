#pragma once

#include "../types.h"
#include "coordinate.h"
#include "world_constants.h"
#include <array>

class ChunkManager;

template <typename T> using BlockArray = std::array<T, CHUNK_VOLUME>;

class Chunk {
  public:
    using Blocks = BlockArray<block_t>;

    Chunk(ChunkManager *manager, const ChunkPosition &position);

    block_t qGetBlock(const BlockPosition &blockPosition) const;
    void qSetBlock(const BlockPosition &blockPosition, block_t block);

    block_t getBlock(const BlockPosition &blockPosition) const;

    const ChunkPosition &getPosition() const;

    Blocks blocks{0};



  private:
    ChunkManager *mp_manager;
    ChunkPosition m_position;
};

class ChunkManager final {
  public:
    Chunk &addChunk(const ChunkPosition &chunk);
    Chunk &addChunk(Chunk &&chunk);

    block_t getBlock(const BlockPosition &blockPosition) const;
    void setBlock(const BlockPosition &blockPosition, block_t block);

    bool hasChunk(const ChunkPosition &chunk) const;
    bool hasNeighbours(const ChunkPosition &chunkPosition) const;

  private:
    ChunkPositionMap<Chunk> m_chunks;
};