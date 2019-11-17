#include "coordinate_convertion.h"

int toChunkBlockIndex(const BlockPosition &blockPosition)
{
    return (blockPosition.y * (CHUNK_SIZE * CHUNK_SIZE) +
            blockPosition.z * CHUNK_SIZE + blockPosition.x);
}

/*
    Block Chunk::getBlock(const BlockPosition &blockPosition) const
    {
        if (blockPosition.x < 0 || blockPosition.x >= CHUNK_SIZE ||
   blockPosition.y < 0 || blockPosition.y >= CHUNK_SIZE || blockPosition.z < 0
   || blockPosition.z >= CHUNK_SIZE) { return BlockType::Air;
        }
        else {
            return m_blocks[(blockPosition.y * (CHUNK_SIZE * CHUNK_SIZE) +
                             blockPosition.z * CHUNK_SIZE + blockPosition.x)];
        }
    }*/