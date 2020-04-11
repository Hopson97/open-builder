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
    Chunk& addChunk(const ChunkPosition& chunk);

    const Chunk& getChunk(const ChunkPosition& chunk);
    const Chunk& getChunk(const ChunkPosition& chunk) const;

    voxel_t getVoxel(const VoxelPosition& voxelPosition) const;
    void setVoxel(const VoxelPosition& voxelPosition, voxel_t voxel);

    /**
     * @brief Check if a chunk exists at this position
     *
     * @param chunk The position to check
     * @return true A chunk exists
     * @return false A chunk does not exist
     */
    bool hasChunk(const ChunkPosition& chunk) const;

    /**
     * @brief Check if a chunk position has all 6 neighbouring chunks
     *
     * @param chunkPosition The position to check neighbours at
     * @return true All 6 neighbours exist
     * @return false Not all 6 neighbours exists
     */
    bool hasNeighbours(const ChunkPosition& chunkPosition) const;

    /**
     * @brief Makes sure a particular chunk has neighbouring chunks
     *
     * @param chunkPosition The chunk to ensure neighbours for
     */
    void ensureNeighbours(const ChunkPosition& chunkPosition);

    const ChunkPositionMap<Chunk>& chunks() const;

  private:
    ChunkPositionMap<Chunk> m_chunks;
};
