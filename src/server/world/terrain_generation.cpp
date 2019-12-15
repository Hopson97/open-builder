#include "terrain_generation.h"

#include <common/world/chunk.h>

void makeFlatTerrain(Chunk *chunk)
{
    if (chunk->getPosition().y < TEMP_WORLD_SIZE - 1 &&
        chunk->getPosition().y > 0 &&
        chunk->getPosition().x < TEMP_WORLD_SIZE - 1 &&
        chunk->getPosition().x > 0 &&
        chunk->getPosition().z < TEMP_WORLD_SIZE - 1 &&
        chunk->getPosition().z > 0) {
        chunk->blocks.fill(1);
    }
}
