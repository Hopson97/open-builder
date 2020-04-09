#include "chunk_mesh_generation.h"

#include <SFML/System/Clock.hpp>
#include <common/world/chunk.h>
#include <iostream>

#include <common/world/voxel_data.h>

namespace {

    const MeshFace FRONT_FACE = {{1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1}, 4};
    const MeshFace LEFT_FACE = {{0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1}, 3};
    const MeshFace BACK_FACE = {{0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0}, 4};
    const MeshFace RIGHT_FACE = {{1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0}, 3};
    const MeshFace TOP_FACE = {{1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1}, 5};
    const MeshFace BOTTOM_FACE = {{0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1}, 2};

    const MeshFace CROSS_FACE_A = {{1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1}, 3};
    const MeshFace CROSS_FACE_B = {{1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0}, 4};

    bool makeFace(const VoxelDataManager& voxelData, voxel_t thisId, voxel_t compareId)
    {
        voxel_t air = voxelData.getVoxelId(CommonVoxel::Air);

        auto& compareVoxel = voxelData.getVoxelData(compareId);
        if (compareId == air) {
            return true;
        }
        else if ((compareVoxel.type != VoxelType::Solid) && (compareId != thisId)) {
            return true;
        }
        return false;
    }

    // @TODO: Generalize this check to any block and any face.
    bool shouldRenderTop(const VoxelDataManager& voxelData,
                         const VoxelData& actualVoxData, voxel_t topVoxel)
    {
        return (actualVoxData.type == VoxelType::Fluid)
                   ? (voxelData.getVoxelData(topVoxel).type != VoxelType::Fluid)
                   : false; // Skips topVoxData check if actualVoxData is not a fluid
    }

} // namespace

ChunkMeshCollection makeChunkMesh(const Chunk& chunk, const VoxelDataManager& voxelData)
{
    // sf::Clock clock;
    // static int count = 0;
    // static float totalTime = 0;
    ChunkMeshCollection meshes(chunk.getPosition());

    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                // If it is "not air"
                VoxelPosition voxelPosition(x, y, z);
                auto voxel = chunk.qGetVoxel(voxelPosition);
                if (voxel > 0) {

                    auto& voxData = voxelData.getVoxelData(voxel);
                    ChunkMesh* mesh = [&meshes, &voxData]() {
                        if (voxData.type == VoxelType::Solid) {
                            return &meshes.voxelMesh;
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
                        mesh->addFace(CROSS_FACE_A, voxelPosition, voxData.sideTextureId);
                        mesh->addFace(CROSS_FACE_B, voxelPosition, voxData.sideTextureId);
                        continue;
                    }

                    // Left voxel face
                    if (makeFace(voxelData, voxel, chunk.getVoxel({x - 1, y, z}))) {
                        mesh->addFace(LEFT_FACE, voxelPosition, voxData.sideTextureId);
                    }

                    // Right chunk face
                    if (makeFace(voxelData, voxel, chunk.getVoxel({x + 1, y, z}))) {
                        mesh->addFace(RIGHT_FACE, voxelPosition, voxData.sideTextureId);
                    }

                    // Front chunk face
                    if (makeFace(voxelData, voxel, chunk.getVoxel({x, y, z + 1}))) {
                        mesh->addFace(FRONT_FACE, voxelPosition, voxData.sideTextureId);
                    }

                    // Back chunk face
                    if (makeFace(voxelData, voxel, chunk.getVoxel({x, y, z - 1}))) {
                        mesh->addFace(BACK_FACE, voxelPosition, voxData.sideTextureId);
                    }

                    // Bottom chunk face
                    if (makeFace(voxelData, voxel, chunk.getVoxel({x, y - 1, z}))) {
                        mesh->addFace(BOTTOM_FACE, voxelPosition,
                                      voxData.bottomTextureId);
                    }

                    auto topVoxel = chunk.getVoxel({x, y + 1, z});

                    // Top chunk face
                    if (makeFace(voxelData, voxel, topVoxel) ||
                        shouldRenderTop(voxelData, voxData, topVoxel)) {
                        mesh->addFace(TOP_FACE, voxelPosition, voxData.topTextureId);
                    }
                }
            }
        }
    }
    // totalTime += clock.getElapsedTime().asSeconds() * 1000.0f;
    // if (count++ % 100 == 0) {
    //    std::cout << count << ' ' << totalTime << ' ' << totalTime / count << '\n';
    //}
    return meshes;
}
