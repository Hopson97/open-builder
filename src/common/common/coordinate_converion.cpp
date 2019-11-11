#include "coordinate_convertion.h"

int toChunkBlockIndex(const BlockPosition &position)
{
    return position.y * CHUNK_AREA + position.z * CHUNK_SIZE + position.x;
}