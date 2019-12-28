#pragma once

#include "../types.h"
#include "coordinate.h"
#include "world_constants.h"
#include <array>
#include <vector>

class ChunkManager;

template <typename T> using BlockArray = std::array<T, CHUNK_VOLUME>;

/**
 * @brief Data structure for a "chunk" of blocks of the game
 *
 */
class Chunk {
  public:
    using Blocks = BlockArray<block_t>;
    using CompressedBlocks = std::vector<std::pair<block_t, u32>>;

    Chunk(ChunkManager &manager, const ChunkPosition &position);

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

    CompressedBlocks compress() const;
    void decompress(const CompressedBlocks &blocks);

    Blocks blocks{0};

  private:
    ChunkManager &mp_manager;
    ChunkPosition m_position;
};
