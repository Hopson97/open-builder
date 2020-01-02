#pragma once

#include <array>
#include <common/world/world_constants.h>
#include <common/world/coordinate.h>

class Chunk;

void makeFlatTerrain(Chunk *chunk);
void makeStepTerrain(Chunk *chunk);
void makeRandomTerrain(Chunk *chunk);

void makeNaturalTerrain(Chunk* chunk, const std::array<int, CHUNK_AREA>& heightMap);


std::array<int, CHUNK_AREA> makeHeightMap(const ChunkPosition &location);