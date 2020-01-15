#pragma once

#include <array>
#include <common/world/coordinate.h>
#include <common/world/world_constants.h>

class Chunk;

void makeFlatTerrain(Chunk *chunk, int worldSize);
void makeStepTerrain(Chunk *chunk);
void makeRandomTerrain(Chunk *chunk);

std::array<int, CHUNK_AREA> createChunkHeightMap(const ChunkPosition &position);

void createSmoothTerrain(Chunk &chunk,
                         const std::array<int, CHUNK_AREA> &heightMap,
                         int worldSize);