#include "terrain_generation.h"

#include <common/world/chunk.h>
#include <cstdlib>
#include <iostream>

#include <ctime>
#include <glm/gtc/noise.hpp>

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

std::array<int, CHUNK_AREA> createChunkHeightMap(const ChunkPosition &position)
{
    int chunkX = position.x;
    int chunkZ = position.z;

    std::array<int, CHUNK_AREA> heightMap;

    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            // Get voxel X/Z positions
            int voxelX = x + chunkX * CHUNK_SIZE;
            int voxelZ = z + chunkZ * CHUNK_SIZE;
            // Get noise value
            float value = glm::simplex(glm::vec2{voxelX / 128.0f, voxelZ / 128.0f});

            // Make it between 0.0 and 1.0
            value = (value + 1) / 2;

            // Make it bigger
            value *= 64;

            int height = value < 10 ? 10 : value;

            //Apply to the height map
            heightMap[z * CHUNK_SIZE + x] = height;
        }
    }
    return heightMap;
}

void createSmoothTerrain(Chunk &chunk,
                         const std::array<int, CHUNK_AREA> &heightMap, int worldSize)
{
    auto cp = chunk.getPosition();
    auto cx = cp.x;
    auto cy = cp.y;
    auto cz = cp.z;

    if (cy < worldSize - 1 && cy > 0 && cx < worldSize - 1 && cx > 0 &&
        cz < worldSize - 1 && cz > 0) {
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