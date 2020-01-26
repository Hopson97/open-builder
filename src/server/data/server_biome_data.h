#pragma once

#include <common/world/world_constants.h>
#include <sol/sol.hpp>
#include <string>
#include <vector>

class VoxelData;

/**
 * @brief Inputs for the noise functions
 *  Used for biome map generation, and height map generation
 */
struct NoiseParameters {
    int octaves = 0;
    float amplitude = 0;
    float smoothness = 0;
    float roughness = 0;
    float offset = 0;
};

/**
 * @brief Data about an individual biome
 * These are created via the Lua, see game/biome_types.lua for examples
 */
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

/**
 * @brief Container class for different biome types
 */
class BiomeData {
  public:
    BiomeData(const VoxelData &voxelData);

    void addBiome(const sol::table &table);

    /**
     * @brief Set the up the "biome map"
     * This is set from the Lua code, and describes where each biome is in the
     * world using the "biome noise map".
     * @param table Lua table containing the biome map information
     */
    void setBiomeMapper(const sol::table &table);

    /**
     * @brief Get the biome ID from a point in the biome map
     * Used for terrain generation, this function will select the biome to use
     * based on 2 things
     * 1. The biome mapper, set from Lua
     * 2. The biome map noise value
     * @param value Noise value from the biome map
     * @return int The BiomeId of the biome for the inputted noise value
     */
    int getBiomeIdFromMapper(float value) const;

    /**
     * @brief Get the BiomeId based on a biome name string
     *
     * @param biomeName The biome name
     * @return int The biome ID for that biome
     */
    int getBiomeId(const std::string &biomeName) const;

    /**
     * @brief Get the BiomeDefintion for that BiomeId
     * 
     * @param biomeId The biome ID
     * @return const BiomeDefinition& The data of that biome
     */
    const BiomeDefinition &getBiomeData(int biomeId) const;

  private:
    std::vector<BiomeDefinition> m_biomes;
    std::vector<std::pair<int, int>> m_biomeMapper;

    const VoxelData *mp_voxelData;
};