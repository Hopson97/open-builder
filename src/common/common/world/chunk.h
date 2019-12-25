#pragma once

#include "../types.h"
#include "coordinate.h"
#include "world_constants.h"
#include <array>

class ChunkManager;

template <typename T> using BlockArray = std::array<T, CHUNK_VOLUME>;

/**
 * @brief Represents a little slice of the world, holding the blocks that make
 * it up
 *
 */
class Chunk {
  public:
    using Blocks = BlockArray<block_t>;

    Chunk(ChunkManager *manager, const ChunkPosition &position);

    /**
     * @brief Quick get block - Gets a block at the local block position without
     * any bounds checking (unsafe)
     *
     * @param blockPosition The position to get the block from
     * @return block_t The block at the position
     */
    block_t qGetBlock(const BlockPosition &blockPosition) const;

    /**
     * @brief Quick set block - Sets a block at the local block position without
     * any bounds checking (unsafe)
     *
     * @param blockPosition The position to set the block from
     * @param block The block to set
     */
    void qSetBlock(const BlockPosition &blockPosition, block_t block);

    /**
     * @brief Get the block at the position
     * This is a SAFE function, as in if you try to get an out-of-bounds chunk
     * it will return a neighbouring chunk's blocks
     * @param blockPosition The world position to get the block from
     * @return block_t The block at this position
     */
    block_t getBlock(const BlockPosition &blockPosition) const;
    const ChunkPosition &getPosition() const;

    Blocks blocks{0};

  private:
    ChunkManager *mp_manager;
    ChunkPosition m_position;
};
