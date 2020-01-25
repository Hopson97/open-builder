#include "terrain_generation.h"

#include <common/world/chunk.h>
#include <cstdlib>
#include <iostream>

#include "../data/server_biome_data.h"
#include <cstring>
#include <ctime>
#include <functional>
#include <glm/gtc/noise.hpp>
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

// THANKS! Karasa and K.jpg for help with this algo
float rounded(const glm::vec2 &coord)
{
    auto bump = [](float t) {
        return glm::max(0.0f, 1.0f - std::pow(t, 6.0f));
    };
    float b = bump(coord.x) * bump(coord.y);
    return b * 0.9f;
}

float getNoiseAt(const glm::vec2 &voxelPos, const NoiseParameters &options,
                 float seed)
{
    // Begin iterating through the octaves
    float value = 0;
    float accumulatedAmps = 0;
    for (int i = 0; i < options.octaves; i++) {
        float frequency = glm::pow(2.0f, i);
        float amplitude = glm::pow(options.roughness, i);

        float x = voxelPos.x * frequency / options.smoothness;
        float y = voxelPos.y * frequency / options.smoothness;

        float noise = glm::simplex(glm::vec3{seed + x, seed + y, seed});
        noise = (noise + 1.0f) / 2.0f;
        value += noise * amplitude;
        accumulatedAmps += amplitude;
    }
    return value / accumulatedAmps;
}

} // namespace

std::array<int, CHUNK_AREA> createBiomeMap(const ChunkPosition &position,
                                           float seed, const BiomeData &biomes)
{
    NoiseParameters biomeNoise;
    biomeNoise.smoothness = 75;
    biomeNoise.roughness = 0.4;
    biomeNoise.octaves = 3;
    std::array<int, CHUNK_AREA> biomeMap;
    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            float bx = x + position.x * CHUNK_SIZE;
            float bz = z + position.z * CHUNK_SIZE;
            glm::vec2 blockPos{bx, bz};
            float value = getNoiseAt(blockPos, biomeNoise, seed);
            // Todo use the biome map (aka BiomeData) here to get the biome
            // types from the noise output
            biomeMap[z * CHUNK_SIZE + x] = value > 0.5 ? 0 : 1;
        }
    }
    return biomeMap;
}

std::array<int, CHUNK_AREA>
createChunkHeightMap(const ChunkPosition &position,
                     const std::array<int, CHUNK_AREA> &biomeMap,
                     float worldSize, float seed, const BiomeData &biomes)
{
    const float WOLRD_SIZE = worldSize * CHUNK_SIZE;

    // TODO Create biome map

    std::array<int, CHUNK_AREA> heightMap;
    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            float bx = x + position.x * CHUNK_SIZE;
            float bz = z + position.z * CHUNK_SIZE;
            glm::vec2 blockPos{bx, bz};
            glm::vec2 coord =
                (glm::vec2{bx, bz} - WOLRD_SIZE / 2.0f) / WOLRD_SIZE * 2.0f;

            auto &biome = biomes.getBiomeData(biomeMap[z * CHUNK_SIZE + x]);
            auto &pnoise = biome.primaryNoise;
            auto &snoise = biome.secondaryNoise;

            auto primary = getNoiseAt(blockPos, pnoise, seed);
            auto secondary = getNoiseAt(blockPos, snoise, seed);
            auto island = rounded(coord) * 1.25;
            float result = primary * secondary;

            float height =
                (result * pnoise.amplitude + pnoise.offset) * island - 5;

            heightMap[z * CHUNK_SIZE + x] = height;
        }
    }

    return heightMap;
}

void createSmoothTerrain(Chunk &chunk,
                         const std::array<int, CHUNK_AREA> &heightMap,
                         const std::array<int, CHUNK_AREA> &biomeMap,
                         int baseChunk, const BiomeData &biomes)
{
    auto base = chunk.getPosition().y - baseChunk;

    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            int height = heightMap[z * CHUNK_SIZE + x];
            for (int y = 0; y < CHUNK_SIZE; y++) {
                BlockPosition bp{x, y, z};
                int by = base * CHUNK_SIZE + y;

                auto &biome = biomes.getBiomeData(biomeMap[z * CHUNK_SIZE + x]);

                if (by > height) {
                    chunk.qSetBlock(bp, by < WATER_LEVEL ? 4 : 0);
                }
                else if (by == height) {
                    bool isBeach = by < WATER_LEVEL + biome.beachHeight;
                    chunk.qSetBlock(bp, isBeach ? biome.beachVoxel
                                                : biome.topVoxel);
                }
                else if (by > height - biome.depth) {
                    chunk.qSetBlock(bp, biome.undergroundVoxel);
                }
                else {
                    chunk.qSetBlock(bp, 5);
                }
            }
        }
    }
}

void makeFlatTerrain(Chunk *chunk, int worldSize)
{
    auto cp = chunk->getPosition();
    auto cx = cp.x;
    auto cy = cp.y;
    auto cz = cp.z;

    if (cy < worldSize - 1 && cy > 0 && cx < worldSize - 1 && cx > 0 &&
        cz < worldSize - 1 && cz > 0) {
        chunk->blocks.fill(1);
    }
}

void makeStepTerrain(Chunk *chunk)
{
    for (int y = 0; y < CHUNK_SIZE; y++) {
        int realY = y + chunk->getPosition().y * CHUNK_SIZE;
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {

                int height = chunk->getPosition().x * CHUNK_SIZE + x +
                             chunk->getPosition().z * CHUNK_SIZE + z;
                if (height > realY) {
                    chunk->qSetBlock({x, y, z}, 1);
                }
            }
        }
    }
}

void makeRandomTerrain(Chunk *chunk)
{
    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                chunk->qSetBlock({x, y, z}, rand() % 64 > 60 ? 1 : 0);
            }
        }
    }
}

float generateSeed(const std::string &input)
{
    std::hash<std::string> strhash;

    float seed_float;
    uint32_t hash = strhash(input);
    std::memcpy(&seed_float, &hash, sizeof(float));
    return seed_float;
}