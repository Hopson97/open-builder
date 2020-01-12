#pragma once

#include <common/world/voxel_types.h>
#include <glad/glad.h>

struct ClientVoxel {
    std::string name;
    GLuint topTexture = 0;
    GLuint sideTexture = 0;
    GLuint bottomTexture = 0;

    VoxelMeshStyle meshStyle = VoxelMeshStyle::Error;
    VoxelMeshType meshType = VoxelMeshType::Error;

    bool isCollidable = true;
};
