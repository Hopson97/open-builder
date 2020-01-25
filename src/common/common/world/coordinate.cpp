#include "coordinate.h"

#include "world_constants.h"
#include <cmath>

int toLocalBlockIndex(const BlockPosition &position)
{
    return position.y * (CHUNK_AREA) + position.z * CHUNK_SIZE + position.x;
}

ChunkPosition worldToChunkPosition(const glm::vec3 &position)
{
    return toChunkPosition(toBlockPosition(position));
}

ChunkPosition toChunkPosition(const BlockPosition &position)
{
    int x = position.x;
    int y = position.y;
    int z = position.z;
    //@TODO Might be a better way to deal with negative coordinates?
    return {
        x < 0 ? ((x - CHUNK_SIZE) / CHUNK_SIZE) : (x / CHUNK_SIZE),
        y < 0 ? ((y - CHUNK_SIZE) / CHUNK_SIZE) : (y / CHUNK_SIZE),
        z < 0 ? ((z - CHUNK_SIZE) / CHUNK_SIZE) : (z / CHUNK_SIZE),
    };
}

BlockPosition toLocalBlockPosition(const BlockPosition &position)
{
    // Deals with negative coordinates too
    return {(CHUNK_SIZE + (position.x % CHUNK_SIZE)) % CHUNK_SIZE,
            (CHUNK_SIZE + (position.y % CHUNK_SIZE)) % CHUNK_SIZE,
            (CHUNK_SIZE + (position.z % CHUNK_SIZE)) % CHUNK_SIZE};
}

BlockPosition toGlobalBlockPosition(const BlockPosition &blockPosition,
                                    const ChunkPosition &localChunkPosition)
{
    return {localChunkPosition.x * CHUNK_SIZE + blockPosition.x,
            localChunkPosition.y * CHUNK_SIZE + blockPosition.y,
            localChunkPosition.z * CHUNK_SIZE + blockPosition.z};
}

BlockPosition toBlockPosition(const glm::vec3 &vec)
{
    auto x = static_cast<i32>(std::floor(vec.x));
    auto y = static_cast<i32>(std::floor(vec.y));
    auto z = static_cast<i32>(std::floor(vec.z));
    return {x, y, z};
}