#pragma once

#include <common/world/voxel_types.h>
#include <sol/sol.hpp>
#include <string>
#include <vector>

#include <common/world/world_constants.h>

struct ServerVoxel {
    std::string name;
    std::string topTexture;
    std::string sideTexture;
    std::string bottomTexture;

    VoxelMeshStyle meshStyle = VoxelMeshStyle::Block;
    VoxelType meshType = VoxelType::Solid;

    bool isCollidable = true;
};

class VoxelData {
  public:
    void addVoxel(const sol::table &table);

    const std::vector<ServerVoxel> &voxelData() const;

    block_t getVoxelId(std::string &voxelName) const;
    const ServerVoxel &getVoxelData(block_t blockId) const;

  private:
    std::vector<ServerVoxel> m_voxelDataList;
};


