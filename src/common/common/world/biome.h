#pragma once

#include "world_constants.h"
#include <string>
#include <unordered_map>
#include <vector>

struct Biome {
    int id = 0;
    std::string name;
    block_t topVoxel;
    block_t undergroundVoxel;
    int depth;
};

struct BiomeDataManager {
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