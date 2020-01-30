#pragma once

#include <common/world/voxel_data.h>
#include <sol/sol.hpp>
#include <string>
#include <vector>

struct ServerVoxel {
    std::string name;
    std::string topTexture;
    std::string sideTexture;
    std::string bottomTexture;

    VoxelMeshStyle meshStyle = VoxelMeshStyle::Block;
    VoxelType type = VoxelType::Solid;

    bool isCollidable = true;
};

using VoxelDataList = std::vector<ServerVoxel>;

class ServerGameData {
  public:
    ServerGameData();

    void addVoxel(const sol::table &table);

    const VoxelDataList &voxelData() const;

  private:
    VoxelDataList m_voxelDataList;
};