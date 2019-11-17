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
 * @brief Determines if a position is out bounds of the chunk eg x >= CHUNK_SIZE
 *
 * @param position The block position to test
 * @return true It is out of bounds
 * @return false It is not out of bounds
 */
bool positionOutOfChunkBounds(const BlockPosition &position);