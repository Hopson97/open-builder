#pragma once

#include <common/data/voxel_types.h>
#include <string>
#include <unordered_map>
#include <vector>

#include <common/world/world_constants.h>

struct VoxelData {
    std::string name;
    std::string topTexture;
    std::string sideTexture;
    std::string bottomTexture;

    VoxelMeshStyle meshStyle;
    VoxelMeshType meshType;
};

class VoxelRegistry {
  public:
    void addVoxelData(const VoxelData &voxel);

    const VoxelData &getVoxelData(block_t id) const;
    const VoxelData &getVoxelData(const std::string &name) const;
    block_t getVoxelId(const std::string &name) const;

  private:
    std::vector<VoxelData> m_voxels;
    std::unordered_map<std::string, block_t> m_voxelMap;
};