#include "terrain_generation.h"

#include <common/world/biome.h>
#include <common/world/chunk.h>
#include <common/world/voxel_data.h>
#include <cstring>
#include <functional>
#include <glm/gtc/noise.hpp>
#include <iostream>
#include <common/util/random_number_generator.h>

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

float getNoiseAt(const glm::vec2& blockPosition, const glm::vec2& chunkPosition,
                 const NoiseOptions& options, float seed)
{
    // Get voxel X/Z positions
    float voxelX = blockPosition.x + chunkPosition.x * CHUNK_SIZE;
    float voxelZ = blockPosition.y + chunkPosition.y * CHUNK_SIZE;

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

} // namespace

std::array<int, CHUNK_AREA> createChunkHeightMap(const ChunkPosition& position,
                                                 float worldSize, float seed)
{
    const float WOLRD_SIZE = worldSize * CHUNK_SIZE;

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

            glm::vec2 coord = (glm::vec2{bx, bz} - WOLRD_SIZE / 2.0f) / WOLRD_SIZE * 2.0f;

            auto noise = getNoiseAt({x, z}, chunkXZ, firstNoise, seed);
            auto noise2 = getNoiseAt({x, z}, {position.x, position.z}, secondNoise, seed);
            auto island = rounded(coord) * 1.25;
            float result = noise * noise2;

            heightMap[z * CHUNK_SIZE + x] =
                (result * firstNoise.amplitude + firstNoise.offset) * island - 5;
        }
    }

    return heightMap;
}

std::array<int, CHUNK_AREA> createBiomeMap(const ChunkPosition& position, float seed)
{
    NoiseOptions biomeMapNoise;
    biomeMapNoise.amplitude = 100;
    biomeMapNoise.octaves = 4;
    biomeMapNoise.smoothness = 505.f;
    biomeMapNoise.roughness = 0.6f;
    biomeMapNoise.offset = 18;

    std::array<int, CHUNK_AREA> biomeMap;
    glm::vec2 chunkXZ = {position.x, position.z};
    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            auto noise = getNoiseAt({x, z}, chunkXZ, biomeMapNoise, seed);

            biomeMap[z * CHUNK_SIZE + x] = noise * biomeMapNoise.amplitude;
        }
    }
    return biomeMap;
}

void createSmoothTerrain(Chunk& chunk, const std::array<int, CHUNK_AREA>& heightMap,
                         const std::array<int, CHUNK_AREA>& biomeMap,
                         const VoxelDataManager& voxelData,
                         const BiomeDataManager& biomeData, int baseChunk, unsigned seed)
{

    // TO DO: Eventully tree gen chance stuff can be done from lua
    RandomNumberGenerator rng;
    rng.setSeed(seed + chunk.getPosition().x * 16 + chunk.getPosition().z);

    auto base = chunk.getPosition().y - baseChunk;

    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            int height = heightMap[z * CHUNK_SIZE + x];
            int biomeVal = biomeMap[z * CHUNK_SIZE + x];
            auto& biome = biomeData.getBiomeData(biomeVal > 40 ? 0 : 1);
            for (int y = 0; y < CHUNK_SIZE; y++) {
                int blockY = base * CHUNK_SIZE + y;
                block_t block = 0;

                if (blockY > height) {
                    if (blockY < WATER_LEVEL) {
                        block = voxelData.getVoxelId(CommonVoxel::Water);
                    }
                }
                else if (blockY == height) {
                    if (blockY < WATER_LEVEL + 3) {
                        block = voxelData.getVoxelId(CommonVoxel::Sand);
                    }
                    else {
                        int n = rng.nextInt(0, 3000);
                        biome.onTopBlockSet(chunk, x, y + 1, z, n);
                        block = biome.topVoxel;
                    }
                }
                else if (blockY > height - biome.depth) {
                    block = biome.undergroundVoxel;
                }
                else {
                    block = voxelData.getVoxelId(CommonVoxel::Stone);
                }
                if (block > 0) {
                    chunk.qSetBlock({x, y, z}, block);
                }
            }
        }
    }
}
void makeFlatTerrain(Chunk* chunk, int worldSize)
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

void makeStepTerrain(Chunk* chunk)
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

void makeRandomTerrain(Chunk* chunk)
{
    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                chunk->qSetBlock({x, y, z}, rand() % 64 > 60 ? 1 : 0);
            }
        }
    }
}

float generateSeed(const std::string& input)
{
    std::hash<std::string> strhash;

    float seed_float;
    uint32_t hash = strhash(input);
    std::memcpy(&seed_float, &hash, sizeof(float));
    return seed_float;
}
