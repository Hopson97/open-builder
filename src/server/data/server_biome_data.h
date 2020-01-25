#pragma once 

#include <sol/sol.hpp>
#include <common/world/world_constants.h>
#include <string>
#include <vector>

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

    int depth = 0;

    block_t topVoxel = 0;
    block_t undergroundVoxel = 0;
};


class BiomeData {
  public:
    void addBiome(sol::table biomeDefinition);

    int getBiomeId(std::string &voxelName) const;
    const BiomeDefinition &getBiomeData(int biomeId) const;

  private:
    std::vector<BiomeDefinition> m_biomes;
};