#pragma once

#include "../types.h"
#include "coordinate.h"
#include "world_constants.h"
#include <array>
#include <vector>

class ChunkManager;

using VoxelArray = std::array<voxel_t, CHUNK_VOLUME>;

/**
 * @brief Compressed chunk voxel data
 * Contains a voxel, followed by how many voxels are exactly the same after
 * it Eg a chunk like [0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 3, 3, 3, 3,
 * 3, 3, 3, 2, 1] Would get compressed to: [0, 4, 1, 4, 0, 2, 1, 4, 3, 7, 2,
 * 1]
 */
using CompressedVoxels = std::vector<std::pair<voxel_t, u16>>;

struct VoxelUpdate {
    VoxelPosition voxelPosition;
    voxel_t voxel;
};

/**
 * @brief Data structure for a "chunk" of voxels of the game
 *
 */
class Chunk {
  public:
    Chunk(ChunkManager& manager, const ChunkPosition& position);

    /**
     * @brief Quick get voxel - Gets a voxel at the local voxel position without
     * any bounds checking (unsafe)
     *
     * @param voxelPosition The position to get the voxel from
     * @return voxel_t The voxel at the position
     */
    voxel_t qGetVoxel(const VoxelPosition& voxelPosition) const;

    /**
     * @brief Quick set voxel - Sets a voxel at the local voxel position without
     * any bounds checking (unsafe)
     *
     * @param voxelPosition The position to set the voxel from
     * @param voxel The voxel to set
     */
    void qSetVoxel(const VoxelPosition& voxelPosition, voxel_t voxel);

    /**
     * @brief Get the voxel at the position
     * This is a SAFE function, as in if you try to get an out-of-bounds voxel
     * it will return a neighbouring chunk's voxels
     * @param voxelPosition The world position to get the voxel from
     * @return voxel_t The voxel at this position
     */
    voxel_t getVoxel(const VoxelPosition& voxelPosition) const;

    /**
     * @brief Set the voxel at the position
     * This is a SAFE function, as in if you try to set an out-of-bounds voxel
     * it will return a neighbouring chunk's voxelsv
     * @param voxelPosition The world position to get the voxel from
     * @param voxel_t The voxel to set at this position
     */
    void setVoxel(const VoxelPosition& voxelPosition, voxel_t voxel);

    const ChunkPosition& getPosition() const;

    VoxelArray voxels{0};

  private:
    ChunkManager& mp_manager;
    ChunkPosition m_position;
};

/**
 * @brief Compress the voxel data of some voxel data
 *
 * @return CompressedVoxels The compressed voxel data [See:
 * CompressedVoxels]
 */
CompressedVoxels compressVoxelData(const VoxelArray& voxels);

/**
 * @brief Uncompress voxel data into this chunk
 *
 * @param voxels The compressed voxel data [See: CompressedVoxels]
 */
VoxelArray decompressVoxelData(const CompressedVoxels& voxels);
