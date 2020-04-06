#include "voxel_data.h"

#include <iostream>

VoxelDataManager::VoxelDataManager()
    : m_commonVoxels(static_cast<size_t>(CommonVoxel::Count))
{
}

void VoxelDataManager::initCommonVoxelTypes()
{
    m_commonVoxels[(u8)CommonVoxel::Air] = getVoxelId("openbuilder_air");
    m_commonVoxels[(u8)CommonVoxel::Stone] = getVoxelId("openbuilder_stone");
    m_commonVoxels[(u8)CommonVoxel::Sand] = getVoxelId("openbuilder_sand");
    m_commonVoxels[(u8)CommonVoxel::Water] = getVoxelId("openbuilder_water");
}

voxel_t VoxelDataManager::addVoxelData(const VoxelData& voxel)
{
    m_voxelMap.emplace(voxel.name, m_voxels.size());
    auto& data = m_voxels.emplace_back(voxel);
    data.id = static_cast<voxel_t>(m_voxels.size() - 1);
    return data.id;
}

const VoxelData& VoxelDataManager::getVoxelData(voxel_t id) const
{

    return m_voxels.at(id);
}

const VoxelData& VoxelDataManager::getVoxelData(const std::string& name) const
{
    return m_voxels.at(getVoxelId(name));
}

voxel_t VoxelDataManager::getVoxelId(CommonVoxel commonVoxel) const
{
    // @TODO Change to operator[] once can be sure it is working
    return m_commonVoxels.at((u8)commonVoxel);
}

voxel_t VoxelDataManager::getVoxelId(const std::string& name) const
{
    return m_voxelMap.at(name);
}

const std::vector<VoxelData>& VoxelDataManager::getVoxelData() const
{
    return m_voxels;
}
