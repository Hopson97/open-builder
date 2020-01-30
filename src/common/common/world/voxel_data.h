#pragma once

#include "../types.h"
#include <string>

#include <glad/glad.h>

enum class VoxelMeshStyle : u8 {
    Block = 0,
    Cross = 1,

    None = 2,

    Error,
};

enum class VoxelType : u8 {
    Solid = 0,
    Fluid = 1,
    Flora = 2,
    Gas = 3,

    Error,
};

struct VoxelData {
    std::string name;
    std::string topTexture;
    std::string sideTexture;
    std::string bottomTexture;

    //Client only
    GLuint topTexture = 0;
    GLuint sideTexture = 0;
    GLuint bottomTexture = 0;

    VoxelMeshStyle meshStyle = VoxelMeshStyle::Block;
    VoxelType type = VoxelType::Solid;

    bool isCollidable = true;
};

class VoxelDataManager {

};