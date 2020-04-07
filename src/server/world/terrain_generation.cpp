#include "terrain_generation.h"

#include <common/util.h>
#include <common/world/biome.h>
#include <common/world/chunk.h>
#include <common/world/chunk_manager.h>
#include <common/world/voxel_data.h>
#include <cstring>
#include <functional>
#include <glm/gtc/noise.hpp>
#include <iostream>

namespace {

    /*
    float trilinearInterpolation(float blf, float blb, float brf, float brb,
                                 float tlf, float tlb, float trf, float trb,
                                 const glm::vec3 &point)
    {
        return (blf * (1 - point.x) * (1 - point.y) * (1 - point.z)) +
               (brf * point.x * (1 - point.y) * (1 - point.z)) +
               (blb * (1 - point.x) * point.y * (1 - point.z)) +
               (tlf * (1 - point.x) * (1 - point.y) * point.z) +
               (trf * point.x * (1 - point.y) * point.z) +
               (tlb * (1 - point.x) * point.y * point.z) +
               (brb * point.x * point.y * (1 - point.z)) +
               (trb * point.x * point.y * point.z);
    }
    */

    struct NoiseOptions {
        int octaves;
        float amplitude;
        float smoothness;
        float roughness;
        float offset;
    };

    // THANKS! Karasa and K.jpg for help with this algo
    float rounded(const glm::vec2& coord)
    {
        auto bump = [](float t) { return glm::max(0.0f, 1.0f - std::pow(t, 6.0f)); };
        float b = bump(coord.x) * bump(coord.y);
        return b * 0.9f;
    }

    float getNoiseAt(const glm::vec2& voxelPosition, const glm::vec2& chunkPosition,
                     const NoiseOptions& options, int seed)
    {
        // Get voxel X/Z positions
        float voxelX = voxelPosition.x + chunkPosition.x * CHUNK_SIZE;
        float voxelZ = voxelPosition.y + chunkPosition.y * CHUNK_SIZE;

        // Begin iterating through the octaves
        float value = 0;
        float accumulatedAmps = 0;
        for (int i = 0; i < options.octaves; i++) {
            float frequency = glm::pow(2.0f, i);
            float amplitude = glm::pow(options.roughness, i);

            float x = voxelX * frequency / options.smoothness;
            float y = voxelZ * frequency / options.smoothness;

            float noise = glm::simplex(glm::vec3{seed + x, seed + y, seed});
            noise = (noise + 1.0f) / 2.0f;
            value += noise * amplitude;
            accumulatedAmps += amplitude;
        }
        return value / accumulatedAmps;
    }

    std::array<int, CHUNK_AREA> createChunkHeightMap(const ChunkPosition& position,
                                                     int worldSize, int seed)
    {
        const float WOLRD_SIZE = static_cast<float>(worldSize) * CHUNK_SIZE;

        NoiseOptions firstNoise;
        firstNoise.amplitude = 105;
        firstNoise.octaves = 6;
        firstNoise.smoothness = 205.f;
        firstNoise.roughness = 0.58f;
        firstNoise.offset = 18;

        NoiseOptions secondNoise;
        secondNoise.amplitude = 20;
        secondNoise.octaves = 4;
        secondNoise.smoothness = 200;
        secondNoise.roughness = 0.45f;
        secondNoise.offset = 0;

        glm::vec2 chunkXZ = {position.x, position.z};

        std::array<int, CHUNK_AREA> heightMap;
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                float bx = static_cast<float>(x + position.x * CHUNK_SIZE);
                float bz = static_cast<float>(z + position.z * CHUNK_SIZE);

                glm::vec2 coord =
                    (glm::vec2{bx, bz} - WOLRD_SIZE / 2.0f) / WOLRD_SIZE * 2.0f;

                auto noise = getNoiseAt({x, z}, chunkXZ, firstNoise, seed);
                auto noise2 =
                    getNoiseAt({x, z}, {position.x, position.z}, secondNoise, seed);
                auto island = rounded(coord) * 1.25;
                float result = noise * noise2;

                heightMap[z * CHUNK_SIZE + x] =
                    static_cast<int>((result * firstNoise.amplitude + firstNoise.offset) *
                                     island) -
                    5;
            }
        }

        return heightMap;
    }

    std::array<int, CHUNK_AREA> createBiomeMap(const ChunkPosition& position, int seed)
    {
        NoiseOptions biomeMapNoise;
        biomeMapNoise.amplitude = 120;
        biomeMapNoise.octaves = 4;
        biomeMapNoise.smoothness = 200.f;
        biomeMapNoise.roughness = 0.5f;
        biomeMapNoise.offset = 18;

        std::array<int, CHUNK_AREA> biomeMap;
        glm::vec2 chunkXZ = {position.x, position.z};
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                auto noise = getNoiseAt({x, z}, chunkXZ, biomeMapNoise, seed);
                int height = static_cast<int>(noise * biomeMapNoise.amplitude);
                biomeMap[z * CHUNK_SIZE + x] = height;
            }
        }
        return biomeMap;
    }

    void createTerrain(Chunk& chunk, const std::array<int, CHUNK_AREA>& heightMap,
                       const std::array<int, CHUNK_AREA>& biomeMap,
                       const VoxelDataManager& voxelData,
                       const BiomeDataManager& biomeData, unsigned seed)
    {
        RandomNumberGenerator rng(seed + chunk.getPosition().x * 16 +
                                  chunk.getPosition().z);
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                int height = heightMap[z * CHUNK_SIZE + x];
                int biomeVal = biomeMap[z * CHUNK_SIZE + x];
                auto& biome = biomeData.getBiomeData(biomeVal > 50 ? 0 : 1);
                for (int y = 0; y < CHUNK_SIZE; y++) {
                    int voxelY = chunk.getPosition().y * CHUNK_SIZE + y;
                    voxel_t voxel = 0;

                    if (voxelY > height) {
                        if (voxelY < WATER_LEVEL) {
                            voxel = voxelData.getVoxelId(CommonVoxel::Water);
                        }
                    }
                    else if (voxelY == height) {
                        if (voxelY < WATER_LEVEL + 3) {
                            voxel = voxelData.getVoxelId(CommonVoxel::Sand);
                        }
                        else {
                            // Allows lua to override the top voxel (eg use dirt if they
                            // place a tree)
                            chunk.qSetVoxel({x, y, z}, biome.topVoxel);
                            biome.onTopVoxelSet(chunk, x, y + 1, z, rng);
                        }
                    }
                    else if (voxelY > height - biome.depth) {
                        voxel = biome.undergroundVoxel;
                    }
                    else {
                        voxel = voxelData.getVoxelId(CommonVoxel::Stone);
                    }
                    if (voxel > 0) {
                        chunk.qSetVoxel({x, y, z}, voxel);
                    }
                }
            }
        }
    }

} // namespace

std::vector<ChunkPosition> generateTerrain(ChunkManager& chunkManager, int chunkX,
                                           int chunkZ, const VoxelDataManager& voxelData,
                                           const BiomeDataManager& biomeData, int seed,
                                           int worldSize)
{
    ChunkPosition position{chunkX, 0, chunkZ};
    std::vector<ChunkPosition> positions;

    auto heightMap = createChunkHeightMap(position, worldSize, seed);
    auto biomeMap = createBiomeMap(position, 9876);
    int maxHeight = *std::max_element(heightMap.cbegin(), heightMap.cend());

    for (int y = 0; y < std::max(1, maxHeight / CHUNK_SIZE + 1); y++) {
        Chunk& chunk = chunkManager.addChunk({chunkX, y, chunkZ});
        createTerrain(chunk, heightMap, biomeMap, voxelData, biomeData, seed);
        chunkManager.ensureNeighbours(chunk.getPosition());
        positions.emplace_back(chunkX, y, chunkZ);
    }
    return positions;
}

float generateSeed(const std::string& input)
{
    std::hash<std::string> strhash;

    float seed_float;
    uint32_t hash = strhash(input);
    std::memcpy(&seed_float, &hash, sizeof(float));
    return seed_float;
}
