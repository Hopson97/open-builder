#pragma once

#include <common/data/voxel_types.h>
#include <string>
#include <unordered_map>
#include <vector>

#include <common/world/world_constants.h>

struct ServerVoxelData {
    std::string name;
    std::string topTexture;
    std::string sideTexture;
    std::string bottomTexture;

    VoxelMeshStyle meshStyle;
    VoxelMeshType meshType;
};
