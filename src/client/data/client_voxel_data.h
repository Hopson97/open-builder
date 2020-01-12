#pragma once

#include <common/data/voxel_types.h>
#include <glad/glad.h>
#include <iostream>

struct ClientVoxelData {
    std::string name;
    GLuint topTexture;
    GLuint sideTexture;
    GLuint bottomTexture;

    VoxelMeshStyle meshStyle;
    VoxelMeshType meshType;
};
