#pragma once

#include "types.h"

// Network stuff
constexpr port_t PORT = 51666;
constexpr int MIN_CONNECTIONS = 2;
constexpr int MAX_CONNECTIONS = 8;

// Game stuff
constexpr int ENTITY_COUNT = 32;
constexpr int CHUNK_SIZE = 32;
constexpr int CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE;
constexpr int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;