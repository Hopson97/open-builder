#pragma once

#include "../types.h"
#include <SFML/System/Vector3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>

using ChunkPosition = sf::Vector3<i32>;
using VoxelPosition = sf::Vector3<i32>;

struct ChunkPositionHash {
    // http://www.beosil.com/download/CollisionDetectionHashing_VMV03.pdf
    std::size_t operator()(const ChunkPosition& position) const
    {
        return (position.x * 88339) ^ (position.z * 91967) ^ (position.z * 126323);
    }
};

template <typename T>
using ChunkPositionMap = std::unordered_map<ChunkPosition, T, ChunkPositionHash>;

/**
 * @brief Converts a local voxel position to an index of a voxel array
 *
 * @param position Local voxel position of a chunk
 * @return int The voxel array index
 */
int toLocalVoxelIndex(const VoxelPosition& position);

/**
 * @brief Converts world coordinates (Eg player position) to chunk coordinates
 *
 * @param position The world position to convert
 * @return ChunkPosition The chunk position at that world position
 */
ChunkPosition worldToChunkPosition(const glm::vec3& position);

/**
 * @brief Converts a world voxel position to a chunk position
 *
 * @param position The world voxel position
 * @return ChunkPosition The converted chunk position at the voxel position
 */
ChunkPosition toChunkPosition(const VoxelPosition& position);

/**
 * @brief Converts a world voxel position to a chunk position
 *
 * @param position The world voxel position
 * @return ChunkPosition The converted chunk position at the voxel position
 */
ChunkPosition toChunkPosition(float xp, float yp, float zp);

/**
 * @brief Converts a world voxel position to a local chunk voxel position
 *
 * @param position The world voxel position to convert
 * @return VoxelPosition The converted local-chunk voxel position
 */
VoxelPosition toLocalVoxelPosition(float xp, float yp, float zp);

/**
 * @brief Converts a world voxel position to a local chunk voxel position
 *
 * @param position The world voxel position to convert
 * @return VoxelPosition The converted local-chunk voxel position
 */
VoxelPosition toLocalVoxelPosition(const VoxelPosition& position);

/**
 * @brief Converts a local voxel position and chunk position to world-voxel
 * position
 *
 * @param voxelPosition The local voxel position
 * @param localChunkPosition The chunk position
 * @return VoxelPosition The world voxel position at those coordinates
 */
VoxelPosition toGlobalVoxelPosition(const VoxelPosition& voxelPosition,
                                    const ChunkPosition& localChunkPosition);

/**
 * @brief Converts world position to a world voxel position
 *
 * @param vec The world position to convert
 * @return VoxelPosition The voxel coordinate at that world position
 */
VoxelPosition toVoxelPosition(const glm::vec3& vec);
