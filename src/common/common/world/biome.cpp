#include "biome.h"

int BiomeDataManager::addBiomeData(const Biome& biome)
{
    m_biomeMap.emplace(biome.name, m_biomes.size());
    auto& data = m_biomes.emplace_back(biome);
    data.id = m_biomes.size() - 1;
    return data.id;
}

const Biome& BiomeDataManager::getBiomeData(int id) const
{
    return m_biomes.at(id);
}

const Biome& BiomeDataManager::getBiomeData(const std::string& name) const
{
    return m_biomes.at(getBiomeId(name));
}

int BiomeDataManager::getBiomeId(const std::string& name) const
{
    return m_biomeMap.at(name);
}

const std::vector<Biome>& BiomeDataManager::getBiomeData() const
{
    return m_biomes;
}
