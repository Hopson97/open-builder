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

/**
 * @brief Basic chunk container
 *
 */
class ChunkManager final {
  public:
    /**
     * @brief Adds a chunk to the position, and returns it
     *
     * @param chunk The position to add a chunk to
     * @return Chunk& The newly added chunk, or the one if one already existed
     */
    Chunk &addChunk(const ChunkPosition &chunk);

    const Chunk &getChunk(const ChunkPosition &chunk);

    block_t getBlock(const BlockPosition &blockPosition) const;
    void setBlock(const BlockPosition &blockPosition, block_t block);

    /**
     * @brief Check if a chunk exists at this position
     *
     * @param chunk The position to check
     * @return true A chunk exists
     * @return false A chunk does not exist
     */
    bool hasChunk(const ChunkPosition &chunk) const;

    /**
     * @brief Check if a chunk position has all 6 neighbouring chunks
     *
     * @param chunkPosition The position to check neighbours at
     * @return true All 6 neighbours exist
     * @return false Not all 6 neighbours exists
     */
    bool hasNeighbours(const ChunkPosition &chunkPosition) const;

  private:
    ChunkPositionMap<Chunk> m_chunks;
};