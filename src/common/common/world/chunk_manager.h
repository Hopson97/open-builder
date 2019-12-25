#pragma once

#include "chunk.h"

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