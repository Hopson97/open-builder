#pragma once

#include "voxel_data.h"
#include <SFML/Network/Packet.hpp>
#include <vector>

class DataRegistry {
  public:
    void addVoxelData(const VoxelData &voxel);

    const VoxelData &getVoxelData(block_t id);

  private:
    std::vector<VoxelData> m_voxels;
};