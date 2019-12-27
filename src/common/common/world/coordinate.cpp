#include "coordinate.h"

#include "world_constants.h"

int toLocalBlockIndex(const BlockPosition &position)
{
    return position.y * (CHUNK_AREA) + position.z * CHUNK_SIZE + position.x;
}

ChunkPosition worldToChunkPosition(const glm::vec3 &position)
{
    float x = static_cast<i32>(position.x);
    float y = static_cast<i32>(position.y);
    float z = static_cast<i32>(position.z);

    return toChunkPosition({x, y, z});
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
    return {static_cast<i32>(vec.x), static_cast<i32>(vec.y),
            static_cast<i32>(vec.z)};
}