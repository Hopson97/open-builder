#include "coordinate_convertion.h"

int toChunkBlockIndex(const BlockPosition &blockPosition)
{
    return (blockPosition.y * (CHUNK_SIZE * CHUNK_SIZE) +
            blockPosition.z * CHUNK_SIZE + blockPosition.x);
}


BlockPosition localBlockToWorldBlockPostion(const BlockPosition &block,
                                            const ChunkSectionPosition &chunk)
{
    return {chunk.x * CHUNK_SIZE + block.x, chunk.y * CHUNK_SIZE + block.y,
            chunk.z * CHUNK_SIZE + block.z};
}

BlockPosition worldBlockToChunkBlockPosition(const BlockPosition& position)
{
    return
    {
        position.x % CHUNK_SIZE,
        position.y,
        position.z % CHUNK_SIZE
    };
}

ChunkPosition worldBlockToChunkPosition(const BlockPosition& position)
{
    return
    {
        position.x / CHUNK_SIZE,
        position.z / CHUNK_SIZE
    };
}

bool positionOutOfChunkBounds(const BlockPosition &blockPosition)
{
    return blockPosition.x < 0 || blockPosition.x >= CHUNK_SIZE ||
           blockPosition.y < 0 || blockPosition.y >= CHUNK_SIZE ||
           blockPosition.z < 0 || blockPosition.z >= CHUNK_SIZE;
}