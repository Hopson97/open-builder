#pragma once

#include "../types.h"
#include "../world/world_constants.h"
#include <string>
#include <unordered_map>
#include <vector>

enum class VoxelMeshStyle : u8 {
    Block = 0,
    Cross = 1,

    None = 2,

    ERROR
};

enum class VoxelMeshType : u8 {
    Solid = 0,
    Fluid = 1,
    Flora = 2,
    Gas = 3,

    ERROR
};

VoxelMeshType toVoxelMeshType(const std::string &meshType);
VoxelMeshStyle toVoxelMeshStyle(const std::string &styleType);

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