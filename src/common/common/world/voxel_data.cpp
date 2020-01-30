#include "voxel_data.h"

block_t VoxelDataManager::addVoxelData(const VoxelData &voxel)
{
    m_voxelMap.emplace(voxel.name, m_voxels.size());
    auto &data = m_voxels.emplace_back(voxel);
    data.id = m_voxels.size() - 1;
    return data.id;
}

const VoxelData &VoxelDataManager::getVoxelData(block_t id) const
{
    return m_voxels.at(id);
}

const VoxelData &VoxelDataManager::getVoxelData(const std::string &name) const
{
    return m_voxels.at(getVoxelId(name));
}

block_t VoxelDataManager::getVoxelId(const std::string &name) const
{
    return m_voxelMap.at(name);
}

const std::vector<VoxelData> &VoxelDataManager::getVoxelData() const
{
    return m_voxels;
}
