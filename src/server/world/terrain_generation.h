#pragma once

#include <array>
#include <common/world/coordinate.h>
#include <common/world/world_constants.h>

class ChunkManager;
class BiomeDataManager;
class VoxelDataManager;

void generateTerrain(ChunkManager& chunkManager, int chunkX, int chunkZ,
                     const VoxelDataManager& voxelData, const BiomeDataManager& biomeData,
                     int seed, int worldSize);

float generateSeed(const std::string& input);