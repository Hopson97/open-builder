#include "terrain_generation.h"

#include <common/world/chunk.h>
#include <cstdlib>
#include <iostream>

void makeFlatTerrain(Chunk *chunk)
{
    if (chunk->getPosition().y < TEMP_WORLD_HEIGHT - 1 &&
        chunk->getPosition().y > 0 &&
        chunk->getPosition().x < TEMP_WORLD_WIDTH - 1 &&
        chunk->getPosition().x > 0 &&
        chunk->getPosition().z < TEMP_WORLD_WIDTH - 1 &&
        chunk->getPosition().z > 0) {

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
