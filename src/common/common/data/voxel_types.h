#pragma once

#include "../types.h"
#include <string>

enum class VoxelMeshStyle : u8 {
    Block = 0,
    Cross = 1,

    ERROR
};

enum class VoxelMeshType : u8 {
    Solid = 0,
    Fluid = 1,
    Flora = 2,

    ERROR
};

VoxelMeshType toVoxelMeshType(const std::string &meshType);
VoxelMeshStyle toVoxelMeshStyle(const std::string &styleType);