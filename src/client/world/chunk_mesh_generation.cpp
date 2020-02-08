#include "chunk_mesh_generation.h"

#include <SFML/System/Clock.hpp>
#include <common/world/chunk.h>
#include <iostream>

#include <common/world/voxel_data.h>

namespace {

const MeshFace FRONT_FACE = {{1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1}, 4};
const MeshFace LEFT_FACE = {{0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, 3};
const MeshFace BACK_FACE = {{0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0}, 3};
const MeshFace RIGHT_FACE = {{1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0}, 3};
const MeshFace TOP_FACE = {{1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1}, 5};
const MeshFace BOTTOM_FACE = {{0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1}, 2};

const MeshFace CROSS_FACE_A = {{1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1}, 3};
const MeshFace CROSS_FACE_B = {{1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0}, 4};

bool makeFace(const VoxelDataManager& voxelData, block_t thisId, block_t compareId)
{
    block_t air = voxelData.getVoxelId(CommonVoxel::Air);

    auto& thisBlock = voxelData.getVoxelData(thisId);
    auto& compareBlock = voxelData.getVoxelData(compareId);
    if (compareId == air) {
        return true;
    }
    else if ((compareBlock.type != VoxelType::Solid) && (compareId != thisId)) {
        return true;
    }
    return false;
}

} // namespace

ChunkMeshCollection makeChunkMesh(const Chunk& chunk, const VoxelDataManager& voxelData)
{
    sf::Clock clock;
    ChunkMeshCollection meshes(chunk.getPosition());

    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                // If it is "not air"
                BlockPosition blockPosition(x, y, z);
                auto voxel = chunk.qGetBlock(blockPosition);
                if (voxel > 0) {

                    auto& voxData = voxelData.getVoxelData(voxel);
                    ChunkMesh* mesh = [&meshes, &voxData]() {
                        if (voxData.type == VoxelType::Solid) {
                            return &meshes.blockMesh;
                        }
                        else if (voxData.type == VoxelType::Flora) {
                            return &meshes.floraMesh;
                        }
                        else if (voxData.type == VoxelType::Fluid) {
                            return &meshes.fluidMesh;
                        }
                        throw std::runtime_error("Unknown voxel type?");
                    }();

                    if (voxData.meshStyle == VoxelMeshStyle::Cross) {
                        mesh->addFace(CROSS_FACE_A, blockPosition, voxData.sideTextureId);
                        mesh->addFace(CROSS_FACE_B, blockPosition, voxData.sideTextureId);
                        continue;
                    }

                    // Left block face
                    if (makeFace(voxelData, voxel, chunk.getBlock({x - 1, y, z}))) {
                        mesh->addFace(LEFT_FACE, blockPosition, voxData.sideTextureId);
                    }

                    // Right chunk face
                    if (makeFace(voxelData, voxel, chunk.getBlock({x + 1, y, z}))) {
                        mesh->addFace(RIGHT_FACE, blockPosition, voxData.sideTextureId);
                    }

                    // Front chunk face
                    if (makeFace(voxelData, voxel, chunk.getBlock({x, y, z + 1}))) {
                        mesh->addFace(FRONT_FACE, blockPosition, voxData.sideTextureId);
                    }

                    // Back chunk face
                    if (makeFace(voxelData, voxel, chunk.getBlock({x, y, z - 1}))) {
                        mesh->addFace(BACK_FACE, blockPosition, voxData.sideTextureId);
                    }

                    // Bottom chunk face
                    if (makeFace(voxelData, voxel, chunk.getBlock({x, y - 1, z}))) {
                        mesh->addFace(BOTTOM_FACE, blockPosition,
                                      voxData.bottomTextureId);
                    }

                    // Top chunk face
                    if (makeFace(voxelData, voxel, chunk.getBlock({x, y + 1, z}))) {
                        mesh->addFace(TOP_FACE, blockPosition, voxData.topTextureId);
                    }
                }
            }
        }
    }
    // std::cout << clock.getElapsedTime().asMilliseconds() << std::endl;

    return meshes;
}
