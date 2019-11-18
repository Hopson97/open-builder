#pragma once

#include "constants.h"

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
 * @brief Determines if a position is out bounds of the chunk eg x >= CHUNK_SIZE
 *
 * @param position The block position to test
 * @return true It is out of bounds
 * @return false It is not out of bounds
 */
bool positionOutOfChunkBounds(const BlockPosition &position);