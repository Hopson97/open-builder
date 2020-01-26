#pragma once

#include <common/world/voxel_types.h>
#include <sol/sol.hpp>
#include <string>
#include <vector>

#include <common/world/world_constants.h>

/**
 * @brief Information about a type of voxel in the world
 * This is set via the Lua, see game/voxel_types.lua for examples
 */
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

    /**
     * @brief Returns all voxels for serialisation purposes
     * @return const std::vector<ServerVoxel>& The voxel data
     */
    const std::vector<ServerVoxel> &voxelData() const;

    block_t getVoxelId(std::string &voxelName) const;
    const ServerVoxel &getVoxelData(block_t blockId) const;

  private:
    std::vector<ServerVoxel> m_voxelDataList;
};
