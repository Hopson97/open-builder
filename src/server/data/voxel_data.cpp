#include "voxel_data.h"

//@TODO Change the .at to be operator []

void VoxelRegistry::addVoxelData(const VoxelData &voxel)
{
    m_voxelMap.emplace(voxel.name, m_voxels.size());
    m_voxels.push_back(voxel);
}

const VoxelData &VoxelRegistry::getVoxelData(block_t id) const
{
    return m_voxels.at(id);
}

const VoxelData &VoxelRegistry::getVoxelData(const std::string &name) const
{
    return m_voxels.at(getVoxelId(name));
}

block_t VoxelRegistry::getVoxelId(const std::string &name) const
{
    return m_voxelMap.at(name);
}
