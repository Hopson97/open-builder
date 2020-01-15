#pragma once

#include <unordered_map>
#include <vector>

#include "../world/world_constants.h"

//@TODO Change the .at to be operator []
template <typename VoxelDataType> class VoxelRegistry {
  public:
    void addVoxelData(const VoxelDataType &voxel)
    {
        m_voxelMap.emplace(voxel.name, m_voxels.size());
        m_voxels.push_back(voxel);
    }

    const VoxelDataType &getVoxelData(block_t id) const
    {
        return m_voxels.at(id);
    }

    const VoxelDataType &getVoxelData(const std::string &name) const
    {
        return m_voxels.at(getVoxelId(name));
    }

    block_t getVoxelId(const std::string &name) const
    {
        return m_voxelMap.at(name);
    }

    const std::vector<VoxelDataType> &getVoxelData() const
    {
        return m_voxels;
    }

  private:
    std::vector<VoxelDataType> m_voxels;
    std::unordered_map<std::string, block_t> m_voxelMap;
};