#pragma once

#include <common/world/voxel_types.h>
#include <sol/sol.hpp>
#include <string>
#include <vector>

struct ServerVoxel {
    std::string name;
    std::string topTexture;
    std::string sideTexture;
    std::string bottomTexture;

    VoxelMeshStyle meshStyle;
    VoxelType meshType;

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