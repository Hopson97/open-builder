#include "chunk_mesh_generation.h"

#include <SFML/System/Clock.hpp>
#include <common/world/chunk.h>
#include <iostream>

#include <common/world/voxel_data.h>

namespace {

const MeshFace FRONT_FACE = {{1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1}, 0.8f};
const MeshFace LEFT_FACE = {{0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, 0.6f};
const MeshFace BACK_FACE = {{0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0}, 0.8f};
const MeshFace RIGHT_FACE = {{1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0}, 0.6f};
const MeshFace TOP_FACE = {{1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1}, 1.0f};
const MeshFace BOTTOM_FACE = {{0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1}, 0.4f};

bool makeFace(const VoxelDataManager &voxelData, block_t thisId,
              block_t compareId)
{
    auto &thisBlock = voxelData.getVoxelData(thisId);
    auto &compareBlock = voxelData.getVoxelData(compareId);
    return (compareBlock.id == 0 || compareBlock.id == 4) &&
           thisBlock.id != compareBlock.id;
}

} // namespace

ChunkMeshCollection makeChunkMesh(const Chunk &chunk,
                                  const VoxelDataManager &voxelData)
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

                    auto &voxData = voxelData.getVoxelData(voxel);
                    ChunkMesh *mesh = voxData.type == VoxelType::Solid
                                          ? &meshes.blockMesh
                                          : &meshes.fluidMesh;

                    // Left block face
                    if (makeFace(voxelData, voxel,
                                 chunk.getBlock({x - 1, y, z}))) {
                        mesh->addFace(LEFT_FACE, blockPosition,
                                      voxData.sideTextureId);
                    }

                    // Right chunk face
                    if (makeFace(voxelData, voxel,
                                 chunk.getBlock({x + 1, y, z}))) {
                        mesh->addFace(RIGHT_FACE, blockPosition,
                                      voxData.sideTextureId);
                    }

                    // Front chunk face
                    if (makeFace(voxelData, voxel,
                                 chunk.getBlock({x, y, z + 1}))) {
                        mesh->addFace(FRONT_FACE, blockPosition,
                                      voxData.sideTextureId);
                    }

                    // Back chunk face
                    if (makeFace(voxelData, voxel,
                                 chunk.getBlock({x, y, z - 1}))) {
                        mesh->addFace(BACK_FACE, blockPosition,
                                      voxData.sideTextureId);
                    }

                    // Bottom chunk face
                    if (makeFace(voxelData, voxel,
                                 chunk.getBlock({x, y - 1, z}))) {
                        mesh->addFace(BOTTOM_FACE, blockPosition,
                                      voxData.bottomTextureId);
                    }

                    // Top chunk face
                    if (makeFace(voxelData, voxel,
                                 chunk.getBlock({x, y + 1, z}))) {
                        mesh->addFace(TOP_FACE, blockPosition,
                                      voxData.topTextureId);
                    }
                }
            }
        }
    }
    // std::cout << clock.getElapsedTime().asMilliseconds() << std::endl;

    return meshes;
}
