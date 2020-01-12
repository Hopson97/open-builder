#pragma once

#include <common/world/voxel_types.h>

struct ServerVoxel {
    std::string name;
    std::string topTexture;
    std::string sideTexture;
    std::string bottomTexture;

    VoxelMeshStyle meshStyle;
    VoxelMeshType meshType;

    bool isCollidable = true;
};
