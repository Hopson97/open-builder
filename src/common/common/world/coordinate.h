#pragma once

#include "../types.h"
#include <SFML/System/Vector3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>

using ChunkPosition = sf::Vector3<i32>;
using BlockPosition = sf::Vector3<i32>;

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
 * @brief Converts a local block position to an index of a block array
 *
 * @param position Local block position of a chunk
 * @return int The block array index
 */
int toLocalBlockIndex(const BlockPosition& position);

/**
 * @brief Converts world coordinates (Eg player position) to chunk coordinates
 *
 * @param position The world position to convert
 * @return ChunkPosition The chunk position at that world position
 */
ChunkPosition worldToChunkPosition(const glm::vec3& position);

/**
 * @brief Converts a world block position to a chunk position
 *
 * @param position The world block position
 * @return ChunkPosition The converted chunk position at the block position
 */
ChunkPosition toChunkPosition(const BlockPosition& position);

/**
 * @brief Converts a world block position to a chunk position
 *
 * @param position The world block position
 * @return ChunkPosition The converted chunk position at the block position
 */
ChunkPosition toChunkPosition(float xp, float yp, float zp);

/**
 * @brief Converts a world block position to a local chunk block position
 *
 * @param position The world block position to convert
 * @return BlockPosition The converted local-chunk block position
 */
BlockPosition toLocalBlockPosition(float xp, float yp, float zp);

/**
 * @brief Converts a world block position to a local chunk block position
 *
 * @param position The world block position to convert
 * @return BlockPosition The converted local-chunk block position
 */
BlockPosition toLocalBlockPosition(const BlockPosition& position);

/**
 * @brief Converts a local block position and chunk position to world-block
 * position
 *
 * @param blockPosition The local block position
 * @param localChunkPosition The chunk position
 * @return BlockPosition The world block position at those coordinates
 */
BlockPosition toGlobalBlockPosition(const BlockPosition& blockPosition,
                                    const ChunkPosition& localChunkPosition);

/**
 * @brief Converts world position to a world block position
 *
 * @param vec The world position to convert
 * @return BlockPosition The block coordinate at that world position
 */
BlockPosition toBlockPosition(const glm::vec3& vec);