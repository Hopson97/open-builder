#include "coordinate_convertion.h"

int toChunkBlockIndex(const BlockPosition &blockPosition)
{
    return (blockPosition.y * (CHUNK_SIZE * CHUNK_SIZE) +
            blockPosition.z * CHUNK_SIZE + blockPosition.x);
}

bool positionOutOfChunkBounds(const BlockPosition &blockPosition)
{
    return blockPosition.x < 0 || blockPosition.x >= CHUNK_SIZE ||
           blockPosition.y < 0 || blockPosition.y >= CHUNK_SIZE ||
           blockPosition.z < 0 || blockPosition.z >= CHUNK_SIZE;
}

BlockPosition localBlockToWorldBlockPostion(const BlockPosition &block,
                                            const ChunkPosition &chunk)
{
    return {chunk.x * CHUNK_SIZE + block.x, chunk.y * CHUNK_SIZE + block.y,
            chunk.z * CHUNK_SIZE + block.z};
}