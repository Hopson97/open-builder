#pragma once

#include "constants.h"

/**
 * @brief Converts a local block position to an array index
 * 
 * @param position Position of block to get coordinates of
 * @return int The array index of this position
 */
int toChunkBlockIndex(const BlockPosition &position);