#pragma once

#include "world_constants.h"
#include <string>
#include <unordered_map>
#include <vector>

#include <sol/sol.hpp>

struct Biome {
    int id = 0;
    std::string name;
    voxel_t topVoxel;
    voxel_t undergroundVoxel;
    int depth;

    sol::function onTopVoxelSet;
};

class BiomeDataManager {
  public:
    int addBiomeData(const Biome& biome);

    const Biome& getBiomeData(int id) const;
    const Biome& getBiomeData(const std::string& name) const;

    int getBiomeId(const std::string& name) const;

    const std::vector<Biome>& getBiomeData() const;

  private:
    std::vector<Biome> m_biomes;
    std::unordered_map<std::string, int> m_biomeMap;
};
