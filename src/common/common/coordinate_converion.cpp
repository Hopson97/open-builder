#include "coordinate_convertion.h"

int toChunkBlockIndex(const BlockPosition &position)
{
    return position.y * CHUNK_AREA + position.z * CHUNK_SIZE + position.x;
}

bool positionOutOfChunkBounds(const BlockPosition &position)
{
    return position.x < 0 || position.x >= CHUNK_SIZE || position.y < 0 ||
           position.y >= CHUNK_SIZE || position.z < 0 ||
           position.z >= CHUNK_SIZE;
}