#pragma once

#include <array>
#include <common/world/coordinate.h>
#include <common/world/world_constants.h>

class Chunk;
class BiomeDataManager;
class VoxelDataManager;

void makeFlatTerrain(Chunk* chunk, int worldSize);
void makeStepTerrain(Chunk* chunk);
void makeRandomTerrain(Chunk* chunk);

std::array<int, CHUNK_AREA> createChunkHeightMap(const ChunkPosition& position,
                                                 float worldSize, float seed);

std::array<int, CHUNK_AREA> createBiomeMap(const ChunkPosition& position, float seed);

void createSmoothTerrain(Chunk& chunk, const std::array<int, CHUNK_AREA>& heightMap,
                         const std::array<int, CHUNK_AREA>& biomeMap,
                         const VoxelDataManager& voxelData,
                         const BiomeDataManager& biomeData, int baseChunk, unsigned seed);

float generateSeed(const std::string& input);