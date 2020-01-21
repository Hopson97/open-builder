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

    if (voxelData["type"].valid()) {
        voxel.meshType = voxelData["type"].get<VoxelType>();
    }

    voxel.topTexture = voxelData["render"]["top"].get<std::string>();
    voxel.sideTexture = voxelData["render"]["sides"].get<std::string>();
    voxel.bottomTexture = voxelData["render"]["bottom"].get<std::string>();
    if (voxelData["render"]["mesh"].valid()) {
        voxel.meshStyle = voxelData["render"]["mesh"].get<VoxelMeshStyle>();
    }

    m_voxelDataList.push_back(voxel);
}

const VoxelDataList &ServerGameData::voxelData() const
{
    return m_voxelDataList;
}
