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

float ball(const glm::vec2 &pixel)
{
    const float WOLRD_SIZE = 8 * CHUNK_SIZE;
    glm::vec2 normalised = pixel / glm::vec2(WOLRD_SIZE, WOLRD_SIZE);
    normalised = (normalised * 2.0f - 1.0f);
    float value = glm::dot(normalised, normalised);
    return value;
}

float square(const glm::vec2 &pixel)
{
    const float WOLRD_SIZE = 8 * CHUNK_SIZE;
    glm::vec2 normalised = pixel / glm::vec2(WOLRD_SIZE, WOLRD_SIZE);
    normalised = (normalised * 2.0f - 1.0f);
    float value = glm::max(glm::abs(normalised.x), glm::abs(normalised.y));
    return value;
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

        float noise = glm::simplex(glm::vec2{x, y});
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
    NoiseOptions land;
    land.amplitude = 70;
    land.octaves = 6;
    land.smoothness = 315.f;
    land.roughness = 0.5f;
    land.offset = 40;

    NoiseOptions ocean;
    ocean.amplitude = 50;
    ocean.octaves = 6;
    ocean.smoothness = 150.f;
    ocean.roughness = 0.6f;
    ocean.offset = 15;

    std::array<int, CHUNK_AREA> heightMap;
    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            float noise = getNoiseAt({x, z}, {position.x, position.z}, land);
            float circle = ball(glm::vec2{x + position.x * CHUNK_SIZE,
                                          z + position.z * CHUNK_SIZE});
            float boxy = square(glm::vec2{x + position.x * CHUNK_SIZE,
                                          z + position.z * CHUNK_SIZE});
            float isIsland = 2 - (circle + boxy);

            

            float height = 1 - circle;
            height = glm::max(0.0f, height);
            height *= height;
            height += getNoiseAt({x, z}, {position.x, position.z}, land) * land.amplitude;

            /*
            if (isIsland > 0.4) {
                height = getNoiseAt({x, z}, {position.x, position.z}, land) *
                             land.amplitude +
                         land.offset;
            }
            else {
                height = getNoiseAt({x, z}, {position.x, position.z}, ocean) *
                             ocean.amplitude +
                         ocean.offset;
            }
            */
            /*
                float h = (noise - (boxy + circle) / 2) * options.amplitude +
                          options.offset;
            float newh = (glm::atan((h - 50.0f) / 75) * 75 + 50.f) / 3.0f;

            // heightMap[z * CHUNK_SIZE + x] = static_cast<int>(newh) + 50.0f;
            */
            heightMap[z * CHUNK_SIZE + x] = static_cast<int>(height);
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
                    //chunk.qSetBlock({x, y, z}, blockY < 64 ? 4 : 0);
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
