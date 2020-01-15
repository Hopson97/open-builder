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

// THANKS! Karasa and K.jpg for help with this algo
float rounded(float x, float y)
{
    auto bump = [](float t) { return glm::max(0.0f, 1.0f - t * t); };
    float b = bump(x) * bump(y);
    return b;
}

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

        float noise = glm::simplex(glm::vec3{x, y, 90595.0f});
        noise = (noise + 1.0f) / 2.0f;
        value += noise * amplitude;
        accumulatedAmps += amplitude;
    }

    // return ((value / accumulatedAmps)) * options.amplitude + options.offset;
    return value / accumulatedAmps;
}

} // namespace

std::array<int, CHUNK_AREA> createChunkHeightMap(const ChunkPosition &position)
{
    const float WOLRD_SIZE = 16 * CHUNK_SIZE;

    NoiseOptions land;
    land.amplitude = 120;
    land.octaves = 6;
    land.smoothness = 195.f;
    land.roughness = 0.54f;
    land.offset = 50;

    NoiseOptions land2;
    land2.amplitude = 20;
    land2.octaves = 2;
    land2.smoothness = 100;
    land2.roughness = 0.65f;
    land2.offset = 0;

    std::array<int, CHUNK_AREA> heightMap;
    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            float bx = x + position.x * CHUNK_SIZE;
            float bz = z + position.z * CHUNK_SIZE;

            glm::vec2 coord =
                (glm::vec2{bx, bz} - WOLRD_SIZE / 2.0f) / WOLRD_SIZE * 2.0f;

            auto noise = getNoiseAt({x, z}, {position.x, position.z}, land);
            auto noise2 = getNoiseAt({x, z}, {position.x, position.z}, land2);
            auto island = rounded(coord.x, coord.y) * 1.25;
            float result = (noise * noise2) * island;


            heightMap[z * CHUNK_SIZE + x] =
                static_cast<int>(result * land.amplitude + land.offset);
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

    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            int height = heightMap[z * CHUNK_SIZE + x];
            for (int y = 0; y < CHUNK_SIZE; y++) {
                int blockY = cy * CHUNK_SIZE + y;

                if (blockY > height) {
                    chunk.qSetBlock({x, y, z}, blockY < 64 ? 4 : 0);
                }
                else if (blockY == height) {
                    chunk.qSetBlock({x, y, z}, blockY < 66 ? 5 : 1);
                }
                else if (blockY > height - 5) {
                    chunk.qSetBlock({x, y, z}, 2);
                }
                else {
                    chunk.qSetBlock({x, y, z}, 3);
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
