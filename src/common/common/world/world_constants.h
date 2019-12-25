#pragma once

#include "../types.h"

using block_t = u8;

constexpr int CHUNK_SIZE = 32;
constexpr int CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE;
constexpr int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

constexpr int TEMP_WORLD_HEIGHT = 3;
constexpr int TEMP_WORLD_WIDTH = 16;