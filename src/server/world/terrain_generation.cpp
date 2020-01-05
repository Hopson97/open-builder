#include "terrain_generation.h"

#include <common/world/chunk.h>
#include <cstdlib>
#include <iostream>

#include <ctime>
#include <glm/gtc/noise.hpp>

namespace {
// temp
struct NoiseParameters {
    float octaves;
    float amplitude;
    float smoothness;
    float heightOffset;

    float roughness;
};

float getHeight(int x, int z, int chunkX, int chunkZ,
                const NoiseParameters &noiseParams, int seed) noexcept
{
    float newX = static_cast<float>(x + chunkX * CHUNK_SIZE);
    float newZ = static_cast<float>(z + chunkZ * CHUNK_SIZE);

    float totalValue = 0.0;

    for (auto octave = 0; octave < noiseParams.octaves - 1; octave++) {
        float frequency = glm::pow(2.0f, octave);
        float amplitude = glm::pow(noiseParams.roughness, octave);
        totalValue +=
            glm::simplex(glm::vec2{newX * frequency / noiseParams.smoothness,
                                   newZ * frequency / noiseParams.smoothness}) *
            amplitude;
    }

    auto val = (((totalValue / 2.1f) + 1.2f) * noiseParams.amplitude) +
               noiseParams.heightOffset;

    return static_cast<float>(val > 0 ? val : 10.0f);
}

} // namespace

void makeFlatTerrain(Chunk *chunk)
{
    auto cp = chunk->getPosition();
    auto cx = cp.x;
    auto cy = cp.y;
    auto cz = cp.z;

    if (cy < TEMP_WORLD_SIZE - 1 && cy > 0 && cx < TEMP_WORLD_SIZE - 1 &&
        cx > 0 && cz < TEMP_WORLD_SIZE - 1 && cz > 0) {
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

std::array<int, CHUNK_AREA> makeHeightMap(const ChunkPosition &location)
{

    auto cx = location.x;
    auto cz = location.z;

    NoiseParameters params;
    params.octaves = 8;
    params.amplitude = 75;
    params.smoothness = 300;
    params.heightOffset = 0;
    params.roughness = 0.49f;
    std::array<int, CHUNK_AREA> heightMap;

    auto smoothstep = [](float edge0, float edge1, float x) {
        x = x * x * (3 - 2 * x);
        return (edge0 * x) + (edge1 * (1 - x));
    };

    auto smoothInterpolation = [smoothstep](float bottomLeft, float topLeft,
                                            float bottomRight, float topRight,
                                            float xMin, float xMax, float zMin,
                                            float zMax, float x, float z) {
        float width = xMax - xMin, height = zMax - zMin;
        float xValue = 1 - (x - xMin) / width;
        float zValue = 1 - (z - zMin) / height;

        float a = smoothstep(bottomLeft, bottomRight, xValue);
        float b = smoothstep(topLeft, topRight, xValue);
        return smoothstep(a, b, zValue);
    };

    auto getHeightIn = [&heightMap, cx, cz, &params, smoothInterpolation](
                           int xMin, int zMin, int xMax, int zMax) {
        auto getHeightAt = [cx, cz, &params](int x, int z) {
            return getHeight(x, z, cx, cz, params, 0);
        };

        float bottomLeft = static_cast<float>(getHeightAt(xMin, zMin));
        float bottomRight = static_cast<float>(getHeightAt(xMax, zMin));
        float topLeft = static_cast<float>(getHeightAt(xMin, zMax));
        float topRight = static_cast<float>(getHeightAt(xMax, zMax));

        for (int z = zMin; z < zMax; ++z) {
            for (int x = xMin; x < xMax; ++x) {

                if (x == CHUNK_SIZE)
                    continue;
                if (z == CHUNK_SIZE)
                    continue;

                float h = smoothInterpolation(
                    bottomLeft, topLeft, bottomRight, topRight,
                    static_cast<float>(xMin), static_cast<float>(xMax),
                    static_cast<float>(zMin), static_cast<float>(zMax),
                    static_cast<float>(x), static_cast<float>(z));

                heightMap[x + z * CHUNK_SIZE] = static_cast<int>(h);
            }
        }
    };

    constexpr static auto H = CHUNK_SIZE / 2;
    constexpr static auto Q = CHUNK_SIZE / 4;
    constexpr static auto F = CHUNK_SIZE;

    // Top left
    getHeightIn(0, 0, Q, Q);
    getHeightIn(Q, 0, H, Q);
    getHeightIn(H, 0, Q * 3, Q);
    getHeightIn(Q * 3, 0, F, Q);

    getHeightIn(0, Q, Q, H);
    getHeightIn(Q, Q, H, H);
    getHeightIn(H, Q, Q * 3, H);
    getHeightIn(Q * 3, Q, F + 1, H);

    getHeightIn(0, H, Q, Q * 3);
    getHeightIn(Q, H, H, Q * 3);
    getHeightIn(H, H, Q * 3, Q * 3);
    getHeightIn(Q * 3, H, F + 1, Q * 3);

    getHeightIn(0, Q * 3, Q, F + 1);
    getHeightIn(Q, Q * 3, H, F + 1);
    getHeightIn(H, Q * 3, Q * 3, F);
    getHeightIn(Q * 3, Q * 3, F + 1, F + 1);

    return heightMap;
}

void makeNaturalTerrain(Chunk *chunk,
                        const std::array<int, CHUNK_AREA> &heightMap)
{
    auto cp = chunk->getPosition();
    auto cx = cp.x;
    auto cy = cp.y;
    auto cz = cp.z;

    if (cy < TEMP_WORLD_SIZE - 1 && cy > 0 && cx < TEMP_WORLD_SIZE - 1 &&
        cx > 0 && cz < TEMP_WORLD_SIZE - 1 && cz > 0) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                int height = heightMap[x + z * CHUNK_SIZE];
                for (int y = 0; y < CHUNK_SIZE; y++) {
                    int blockY = (y + CHUNK_SIZE * cy);
                    if (blockY <= height || blockY < 80) {
                        chunk->qSetBlock({x, y, z}, 1);
                    }
                }
            }
        }
    }
}