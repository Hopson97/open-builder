#include "server_voxel_data.h"

#include <common/debug.h>

void VoxelData::addVoxel(const sol::table &voxelData)
{
    ServerVoxel voxel;

    if (voxelData["collidable"].valid()) {
        voxel.isCollidable = voxelData["collidable"].get<bool>();
    }
    if (voxelData["type"].valid()) {
        voxel.meshType = voxelData["type"].get<VoxelType>();
    }
    if (voxelData["render"]["mesh"].valid()) {
        voxel.meshStyle = voxelData["render"]["mesh"].get<VoxelMeshStyle>();
    }

    voxel.name = voxelData["name"].get<std::string>();
    voxel.topTexture = voxelData["render"]["top"].get<std::string>();
    voxel.sideTexture = voxelData["render"]["sides"].get<std::string>();
    voxel.bottomTexture = voxelData["render"]["bottom"].get<std::string>();

    m_voxelDataList.push_back(voxel);
}

const std::vector<ServerVoxel> &VoxelData::voxelData() const
{
    return m_voxelDataList;
}

block_t VoxelData::getVoxelId(std::string &voxelName) const
{
    for (block_t i = 0; i < m_voxelDataList.size(); i++) {
        if (m_voxelDataList[i].name == voxelName) {
            return i;
        }
    }
    // Return air on fail
    return 0;
}

const ServerVoxel &VoxelData::getVoxelData(block_t blockId) const
{
    // TODO Maybe change to operator [] eventually
    return m_voxelDataList.at(blockId);
}