#pragma once

#include <common/data/voxel_types.h>
#include <glad/glad.h>
#include <iostream>

struct ClientVoxelData {
    std::string name;
    GLuint topTexture = 0;
    GLuint sideTexture = 0;
    GLuint bottomTexture = 0;

    VoxelMeshStyle meshStyle = VoxelMeshStyle::Error;
    VoxelMeshType meshType = VoxelMeshType::Error;
};
