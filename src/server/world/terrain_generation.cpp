#include "terrain_generation.h"

#include <common/world/chunk.h>
#include <cstdlib>
#include <iostream>

#include <ctime>

// temp
struct NoiseParameters {
    int octaves;
    int amplitude;
    int smoothness;
    int heightOffset;

    float roughness;
};

class NoiseGenerator {
  public:
    NoiseGenerator(int seed);

    float getHeight(int x, int z, int chunkX, int chunkZ) const noexcept;

    void setParameters(const NoiseParameters &params) noexcept;

  private:
    float getNoise(int n) const noexcept;
    float getNoise(float x, float z) const noexcept;

    float lerp(float a, float b, float z) const noexcept;

    float noise(float x, float z) const noexcept;

    NoiseParameters m_noiseParameters;

    int m_seed;
};

#include <cmath>

NoiseGenerator::NoiseGenerator(int seed)
    : m_seed(seed)
{
    m_noiseParameters.octaves = 7;
    m_noiseParameters.amplitude = 70;
    m_noiseParameters.smoothness = 235;
    m_noiseParameters.heightOffset = -5;
    m_noiseParameters.roughness = 0.53;
}

void NoiseGenerator::setParameters(const NoiseParameters &params) noexcept
{
    m_noiseParameters = params;
}

float NoiseGenerator::getNoise(int n) const noexcept
{
    n += m_seed;
    n = (n << 13) ^ n;
    auto newN = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;

    return 1.0 - ((float)newN / 1073741824.0);
}

float NoiseGenerator::getNoise(float x, float z) const noexcept
{
    return getNoise(x + z * 57.0);
}

float NoiseGenerator::lerp(float a, float b, float z) const noexcept
{
    float mu2 = (1 - std::cos(z * 3.14)) / 2;
    return (a * (1 - mu2) + b * mu2);
}

float NoiseGenerator::noise(float x, float z) const noexcept
{
    auto floorX =
        (float)((int)x); // This is kinda a cheap way to floor a float integer.
    auto floorZ = (float)((int)z);

    auto s = 0.0, t = 0.0, u = 0.0,
         v = 0.0; // Integer declaration

    s = getNoise(floorX, floorZ);
    t = getNoise(floorX + 1, floorZ);
    u = getNoise(
        floorX,
        floorZ + 1); // Get the surrounding values to calculate the transition.
    v = getNoise(floorX + 1, floorZ + 1);

    auto rec1 = lerp(s, t, x - floorX); // Interpolate between the values.
    auto rec2 = lerp(
        u, v,
        x - floorX); // Here we use x-floorX, to get 1st dimension. Don't mind
                     // the x-floorX thingie, it's part of the cosine formula.
    auto rec3 =
        lerp(rec1, rec2,
             z - floorZ); // Here we use y-floorZ, to get the 2nd dimension.
    return rec3;
}

float NoiseGenerator::getHeight(int x, int z, int chunkX, int chunkZ) const
    noexcept
{
    auto newX = (x + (chunkX * CHUNK_SIZE));
    auto newZ = (z + (chunkZ * CHUNK_SIZE));

    auto totalValue = 0.0;

    for (auto a = 0; a < m_noiseParameters.octaves - 1;
         a++) // This loops through the octaves.
    {
        auto frequency = pow(
            2.0,
            a); // This increases the frequency with every loop of the octave.
        auto amplitude = pow(
            m_noiseParameters.roughness,
            a); // This decreases the amplitude with every loop of the octave.
        totalValue +=
            noise(((float)newX) * frequency / m_noiseParameters.smoothness,
                  ((float)newZ) * frequency / m_noiseParameters.smoothness) *
            amplitude;
    }

    auto val = (((totalValue / 2.1) + 1.2) * m_noiseParameters.amplitude) +
               m_noiseParameters.heightOffset;

    return val > 0 ? val : 1;
}

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

void makeNaturalTerrain(Chunk *chunk)
{
    auto cp = chunk->getPosition();
    auto cx = cp.x;
    auto cy = cp.y;
    auto cz = cp.z;

    if (cy < TEMP_WORLD_SIZE - 1 && cy > 0 && cx < TEMP_WORLD_SIZE - 1 &&
        cx > 0 && cz < TEMP_WORLD_SIZE - 1 && cz > 0) {
        NoiseParameters params;
        params.octaves = 5;
        params.amplitude = 285;
        params.smoothness = 300;
        params.heightOffset = -200;
        params.roughness = 0.54;

        NoiseGenerator gen(std::time(nullptr));
        gen.setParameters(params);
        std::array<int, CHUNK_AREA> heightMap;

        auto smoothstep = [](float edge0, float edge1, float x) {
            // Scale, bias and saturate x to 0..1 range
            x = x * x * (3 - 2 * x);
            // Evaluate polynomial
            return (edge0 * x) + (edge1 * (1 - x));
        };

        auto smoothInterpolation =
            [smoothstep](float bottomLeft, float topLeft, float bottomRight,
                         float topRight, float xMin, float xMax, float zMin,
                         float zMax, float x, float z) {
                float width = xMax - xMin, height = zMax - zMin;
                float xValue = 1 - (x - xMin) / width;
                float zValue = 1 - (z - zMin) / height;

                // std::cout << xValue << std::endl;

                float a = smoothstep(bottomLeft, bottomRight, xValue);
                float b = smoothstep(topLeft, topRight, xValue);
                return smoothstep(a, b, zValue);
            };

        auto getHeightIn = [&](int xMin, int zMin, int xMax, int zMax) {
            auto getHeightAt = [&](int x, int z) {
                return gen.getHeight(x, z, cx, cz);
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
        getHeightIn(Q * 3, Q, F, H);

        getHeightIn(0, H, Q, Q * 3);
        getHeightIn(Q, H, H, Q * 3);
        getHeightIn(H, H, Q * 3, Q * 3);
        getHeightIn(Q * 3, H, F, Q * 3);

        getHeightIn(0, Q * 3, Q, F);
        getHeightIn(Q, Q * 3, H, F);
        getHeightIn(H, Q * 3, Q * 3, F);
        getHeightIn(Q * 3, Q * 3, F, F);

        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                int height = heightMap[x + z * CHUNK_SIZE];
                for (int y = 0; y < CHUNK_SIZE; y++) {
                    if ((y + CHUNK_SIZE * cy) < height) {
                        chunk->qSetBlock({x, y, z}, 1);
                    }
                }
            }
        }
    }
}