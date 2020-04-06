#pragma once

#include <array>
#include <common/world/coordinate.h>
#include <common/world/world_constants.h>
#include <vector>

class ChunkManager;
class BiomeDataManager;
class VoxelDataManager;

std::vector<ChunkPosition> generateTerrain(ChunkManager& chunkManager, int chunkX,
                                           int chunkZ, const VoxelDataManager& voxelData,
                                           const BiomeDataManager& biomeData, int seed,
                                           int worldSize);

float generateSeed(const std::string& input);