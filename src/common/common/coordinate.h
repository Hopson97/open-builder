#pragma once

#include "constants.h"

// Game types
using BlockPosition = sf::Vector3<i32>;
using ChunkSectionPosition = sf::Vector3<i32>;

struct ChunkPosition {
    ChunkPosition() = default;
    ChunkPosition(int xp, int zp)
        : x(xp)
        , z(zp)
    {
    }

    i32 x = 0;
    i32 z = 0;
};

struct ChunkPositionHash {
    // http://www.beosil.com/download/CollisionDetectionHashing_VMV03.pdf
    std::size_t operator()(const ChunkPosition &position) const
    {
        return (position.x * 130199) ^ (position.z * 146437);
    }
};

template <typename T>
using ChunkPositionMap =
    std::unordered_map<ChunkPosition, T, ChunkPositionHash>;

/**
 * @brief Converts a local block position to an array index
 *
 * @param position Position of block to get coordinates of
 * @return int The array index of this position
 */
int toChunkBlockIndex(const BlockPosition &position);

/**
 * @brief Converts a block position local to a chunk to world block position
 *
 * @param block The local chunk block position
 * @param chunk The chunk position
 * @return BlockPosition The new, converted, block position
 */
BlockPosition localBlockToWorldBlockPostion(const BlockPosition &block,
                                            const ChunkSectionPosition &chunk);

/**
 * @brief Converts a world block position to a chunk block position
 *
 * @param position The world block position to convert
 * @return BlockPosition The converted chunk block position
 */
BlockPosition worldBlockToChunkBlockPosition(const BlockPosition &position);

/**
 * @brief Converts a block position in the world to a chunk position (XZ plane)
 *
 * @param position The block position to convert
 * @return ChunkPosition The converted chunk position
 */
ChunkPosition worldBlockToChunkPosition(const BlockPosition &position);

/**
 * @brief Determines if a position is out bounds of the chunk eg x >= CHUNK_SIZE
 *
 * @param position The block position to test
 * @return true It is out of bounds
 * @return false It is not out of bounds
 */
bool positionOutOfChunkBounds(const BlockPosition &position);