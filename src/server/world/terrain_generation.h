#pragma once

#include <array>
#include <common/world/coordinate.h>
#include <common/world/world_constants.h>

class Chunk;
class BiomeData;

void makeFlatTerrain(Chunk *chunk, int worldSize);
void makeStepTerrain(Chunk *chunk);
void makeRandomTerrain(Chunk *chunk);

std::array<int, CHUNK_AREA> createBiomeMap(const ChunkPosition &position,
                                           float seed, const BiomeData &biomes);

std::array<int, CHUNK_AREA>
createChunkHeightMap(const ChunkPosition &position,
                     const std::array<int, CHUNK_AREA> &biomeMap,
                     float worldSize, float seed, const BiomeData &biomes);

void createSmoothTerrain(Chunk &chunk,
                         const std::array<int, CHUNK_AREA> &heightMap,
                         const std::array<int, CHUNK_AREA> &biomeMap,
                         int baseChunk, const BiomeData &biomes);

float generateSeed(const std::string &input);