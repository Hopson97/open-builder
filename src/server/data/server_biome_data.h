#pragma once

#include <common/world/world_constants.h>
#include <sol/sol.hpp>
#include <string>
#include <vector>

class VoxelData;

struct NoiseParameters {
    int octaves = 0;
    float amplitude = 0;
    float smoothness = 0;
    float roughness = 0;
    float offset = 0;
};

struct BiomeDefinition {
    NoiseParameters primaryNoise;
    NoiseParameters secondaryNoise;

    std::string name = "Unknown";
    std::string description = "N/A";

    int depth = 0;
    int beachHeight = 0;

    block_t topVoxel = 0;
    block_t undergroundVoxel = 0;
    block_t beachVoxel = 0;
};

class BiomeData {
  public:
    BiomeData(const VoxelData &voxelData);
    void addBiome(const sol::table &table);

    int getBiomeId(std::string &voxelName) const;
    const BiomeDefinition &getBiomeData(int biomeId) const;

  private:
    std::vector<BiomeDefinition> m_biomes;

    const VoxelData *mp_voxelData;
};