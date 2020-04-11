#include "coordinate.h"

#include "world_constants.h"
#include <cmath>

int toLocalVoxelIndex(const VoxelPosition& position)
{
    return position.y * (CHUNK_AREA) + position.z * CHUNK_SIZE + position.x;
}

ChunkPosition worldToChunkPosition(const glm::vec3& position)
{
    return toChunkPosition(toVoxelPosition(position));
}

ChunkPosition toChunkPosition(const VoxelPosition& position)
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

ChunkPosition toChunkPosition(float xp, float yp, float zp)
{
    int x = static_cast<int>(xp);
    int y = static_cast<int>(yp);
    int z = static_cast<int>(zp);
    return toChunkPosition({x, y, z});
}

VoxelPosition toLocalVoxelPosition(float xp, float yp, float zp)
{
    int x = static_cast<int>(xp);
    int y = static_cast<int>(yp);
    int z = static_cast<int>(zp);
    return toLocalVoxelPosition({x, y, z});
}

VoxelPosition toLocalVoxelPosition(const VoxelPosition& position)
{
    // Deals with negative coordinates too
    return {(CHUNK_SIZE + (position.x % CHUNK_SIZE)) % CHUNK_SIZE,
            (CHUNK_SIZE + (position.y % CHUNK_SIZE)) % CHUNK_SIZE,
            (CHUNK_SIZE + (position.z % CHUNK_SIZE)) % CHUNK_SIZE};
}

VoxelPosition toGlobalVoxelPosition(const VoxelPosition& voxelPosition,
                                    const ChunkPosition& localChunkPosition)
{
    return {localChunkPosition.x * CHUNK_SIZE + voxelPosition.x,
            localChunkPosition.y * CHUNK_SIZE + voxelPosition.y,
            localChunkPosition.z * CHUNK_SIZE + voxelPosition.z};
}

VoxelPosition toVoxelPosition(const glm::vec3& vec)
{
    auto x = static_cast<i32>(std::floor(vec.x));
    auto y = static_cast<i32>(std::floor(vec.y));
    auto z = static_cast<i32>(std::floor(vec.z));
    return {x, y, z};
}
