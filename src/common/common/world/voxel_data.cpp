#include "voxel_data.h"

VoxelDataManager::VoxelDataManager()
    : m_commonVoxels(static_cast<size_t>(CommonVoxel::Count))
{
}

void VoxelDataManager::initCommonVoxelTypes()
{
    // TODO Eventually some of this won't be needed as "common voxels" as this
    // stuff
    // would be delagated to the Lua instead
    // For now though, this can work as a placeholder
    m_commonVoxels[(u8)CommonVoxel::Air] = getVoxelId("openbuilder_air");
    m_commonVoxels[(u8)CommonVoxel::Stone] = getVoxelId("openbuilder_stone");
    m_commonVoxels[(u8)CommonVoxel::Sand] = getVoxelId("openbuilder_sand");
    m_commonVoxels[(u8)CommonVoxel::Water] = getVoxelId("openbuilder_water");
}

block_t VoxelDataManager::addVoxelData(const VoxelData& voxel)
{
    m_voxelMap.emplace(voxel.name, m_voxels.size());
    auto& data = m_voxels.emplace_back(voxel);
    data.id = static_cast<block_t>(m_voxels.size() - 1);
    return data.id;
}

const VoxelData& VoxelDataManager::getVoxelData(block_t id) const
{
    return m_voxels.at(id);
}

const VoxelData& VoxelDataManager::getVoxelData(const std::string& name) const
{
    return m_voxels.at(getVoxelId(name));
}

block_t VoxelDataManager::getVoxelId(CommonVoxel commonVoxel) const
{
    // TODO Change to operator[] once can be sure it is working
    return m_commonVoxels.at((u8)commonVoxel);
}

block_t VoxelDataManager::getVoxelId(const std::string& name) const
{
    return m_voxelMap.at(name);
}

const std::vector<VoxelData>& VoxelDataManager::getVoxelData() const
{
    return m_voxels;
}
