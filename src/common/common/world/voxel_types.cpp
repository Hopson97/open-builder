#include "voxel_types.h"

#include "../debug.h"

VoxelMeshType toVoxelMeshType(const std::string &meshType)
{
    if (meshType == "solid") {
        return VoxelMeshType::Solid;
    }
    else if (meshType == "flora") {
        return VoxelMeshType::Flora;
    }
    else if (meshType == "fluid") {
        return VoxelMeshType::Fluid;
    }
    else if (meshType == "gas") {
        return VoxelMeshType::Gas;
    }
    else {
        LOGVAR("Loading", "Unkown mesh type: ", meshType);
        return VoxelMeshType::Error;
    }
}

VoxelMeshStyle toVoxelMeshStyle(const std::string &styleType)
{
    if (styleType == "block") {
        return VoxelMeshStyle::Block;
    }
    else if (styleType == "cross") {
        return VoxelMeshStyle::Cross;
    }
    else if (styleType == "none") {
        return VoxelMeshStyle::None;
    }
    else {
        LOGVAR("Loading", "Unkown mesh style: ", styleType);
        return VoxelMeshStyle::Error;
    }
}