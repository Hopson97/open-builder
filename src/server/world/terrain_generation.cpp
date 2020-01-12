#include "terrain_generation.h"

#include <common/world/chunk.h>
#include <cstdlib>
#include <iostream>

#include <ctime>
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

struct NoiseOptions {
    int octaves;
    float amplitude;
    float smoothness;
    float roughness;
    float offset;
};

float getNoiseAt(const glm::vec2 &blockPosition, const glm::vec2 &chunkPosition,
                 const NoiseOptions &options)
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

        float noise = glm::simplex(glm::vec2{x, y});
        noise = (noise + 1.0f) / 2.0f;
        value += noise * amplitude;
        accumulatedAmps += amplitude;
    }

    return ((value / accumulatedAmps)) * options.amplitude + options.offset;
}

} // namespace

std::array<int, CHUNK_AREA> createChunkHeightMap(const ChunkPosition &position)
{
    NoiseOptions options;
    options.amplitude = 125;
    options.octaves = 6;
    options.smoothness = 215.f;
    options.roughness = 0.5f;
    options.offset = 20.0f;

    std::array<int, CHUNK_AREA> heightMap;
    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            heightMap[z * CHUNK_SIZE + x] = static_cast<int>(
                getNoiseAt({x, z}, {position.x, position.z}, options));
        }
    }
    return heightMap;
}

void createSmoothTerrain(Chunk &chunk,
                         const std::array<int, CHUNK_AREA> &heightMap,
                         int worldSize)
{
    auto cp = chunk.getPosition();
    auto cx = cp.x;
    auto cy = cp.y;
    auto cz = cp.z;

    if (cy > 0 && cx < worldSize - 1 && cx > 0 && cz < worldSize - 1 &&
        cz > 0) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                int height = heightMap[z * CHUNK_SIZE + x];
                for (int y = 0; y < CHUNK_SIZE; y++) {
                    int blockY = cy * CHUNK_SIZE + y;

                    if (blockY <= height) {
                        chunk.qSetBlock({x, y, z}, 1);
                    }
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

void makeRawNoiseTerrain(Chunk &chunk)
{
    // Create a height map
    // Higher noise value = Higher terrain at that location
    std::array<int, CHUNK_AREA> heightMap;
    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            // Get block position in the world (X and Z, as Y is up/height)
            float bx =
                static_cast<float>(chunk.getPosition().x * CHUNK_SIZE + x);
            float bz =
                static_cast<float>(chunk.getPosition().z * CHUNK_SIZE + z);

            // Get noise value
            float noiseValue =
                glm::simplex(glm::vec2{bx / 200.0f, bz / 200.0f});

            // Make it a positive number
            noiseValue = (noiseValue + 1) / 2.0f;

            // Give it some height
            noiseValue *= 32.0f + 64;

            // Apply to height map
            heightMap[z * CHUNK_SIZE + x] = static_cast<int>(noiseValue);
        }
    }

    // Apply height map to the chunk
    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            int height = heightMap[z * CHUNK_SIZE + x];
            for (int y = 0; y < CHUNK_SIZE; y++) {
                int blockY = chunk.getPosition().y * CHUNK_SIZE + y;

                if (blockY <= height) {
                    chunk.qSetBlock({x, y, z}, 1);
                }
            }
        }
    }
}
