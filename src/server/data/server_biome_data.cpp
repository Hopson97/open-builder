#include "server_biome_data.h"

#include "server_voxel_data.h"
#include <iostream>

namespace {
NoiseParameters readNoiseParameters(const sol::table &noise)
{
    NoiseParameters params;
    params.amplitude = noise["amplitude"].get<float>();
    params.octaves = noise["octaves"].get<float>();
    params.offset = noise["heightOffset"].get<float>();
    params.roughness = noise["roughness"].get<float>();
    params.smoothness = noise["smoothness"].get<float>();
    return params;
}
} // namespace

BiomeData::BiomeData(const VoxelData &voxelData)
    : mp_voxelData(&voxelData)
{
}

void BiomeData::addBiome(const sol::table &table)
{
    BiomeDefinition biome;
    biome.primaryNoise = readNoiseParameters(table["primaryNoise"]);
    biome.secondaryNoise = readNoiseParameters(table["secondaryNoise"]);

    biome.name = table["name"].get<std::string>();
    biome.description = table["description"].get<std::string>();
    biome.depth = table["depth"].get<int>();
    biome.beachHeight = table["beachHeight"].get<int>();

    auto topVoxel = table["topVoxel"].get<std::string>();
    auto undergroundVoxel = table["undergroundVoxel"].get<std::string>();
    auto beachVoxel = table["beachVoxel"].get<std::string>();

    biome.topVoxel = mp_voxelData->getVoxelId(topVoxel);
    biome.undergroundVoxel = mp_voxelData->getVoxelId(undergroundVoxel);
    biome.beachVoxel = mp_voxelData->getVoxelId(beachVoxel);

    m_biomes.push_back(biome);
}

void BiomeData::setBiomeMapper(const sol::table &table)
{
    table.for_each([&](sol::object const &key, sol::object const &value) {
        m_biomeMapper.emplace_back(getBiomeId(key.as<std::string>()),
                                   value.as<float>());
    });

    std::sort(m_biomeMapper.begin(), m_biomeMapper.end(),
              [](auto &a, auto &b) { return a.second > b.second; });
}

int BiomeData::getBiomeIdFromMapper(float value) const
{
    // Looks for the biome ID in the biome map that is lowest in comparision to
    // value idk how to explain this actually but it works
    unsigned index = 0;
    if (m_biomeMapper.empty()) {
        return index;
    }
    while (m_biomeMapper[index].second > value) {
        if (index == m_biomeMapper.size()) {
            break;
        }
        index++;
    }
    return m_biomeMapper[index].first;
}

int BiomeData::getBiomeId(const std::string &biomeName) const
{
    for (block_t i = 0; i < m_biomes.size(); i++) {
        if (m_biomes[i].name == biomeName) {
            return i;
        }
    }
    // Return air on fail
    return 0;
}

const BiomeDefinition &BiomeData::getBiomeData(int biomeId) const
{
    // TODO Maybe change to operator [] eventually
    return m_biomes.at(biomeId);
}
