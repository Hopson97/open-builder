#include "server_game_data.h"

#include <common/debug.h>

ServerGameData::ServerGameData()
{
}

void ServerGameData::addVoxel(const sol::table &voxelData)
{
    ServerVoxel voxel;
    voxel.name = voxelData["name"].get<std::string>();

    if (voxelData["collidable"].valid()) {
        voxel.isCollidable = voxelData["collidable"].get<bool>();
    }
    if (voxelData["render"]["mesh"].valid()) {
  
        voxel.meshStyle = voxelData["render"]["mesh"].get<VoxelMeshStyle>();
    }
    if (voxelData["render"]["type"].valid()) {

        voxel.meshType = voxelData["render"]["type"].get<VoxelType>();
    }

    voxel.meshType = voxelData["render"]["type"].get<VoxelType>();
    voxel.topTexture = voxelData["render"]["top"].get<std::string>();
    voxel.sideTexture = voxelData["render"]["sides"].get<std::string>();
    voxel.bottomTexture = voxelData["render"]["bottom"].get<std::string>();

    m_voxelDataList.push_back(voxel);

    LOGVAR("Server", "Adding block", voxel.name);
}

const VoxelDataList &ServerGameData::voxelData() const
{
    return m_voxelDataList;
}
