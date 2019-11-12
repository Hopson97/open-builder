#include "coordinate_convertion.h"

int toChunkBlockIndex(const BlockPosition &position)
{
    return position.y * (CHUNK_SIZE * CHUNK_SIZE) + position.z * CHUNK_SIZE +
           position.x;
}

bool positionOutOfChunkBounds(const BlockPosition &position)
{
    return position.x < 0 || position.x >= CHUNK_SIZE || position.y < 0 ||
           position.y >= CHUNK_SIZE || position.z < 0 ||
           position.z >= CHUNK_SIZE;
}

/*
    Block Chunk::getBlock(const BlockPosition &bposition) const
    {
        if (bposition.x < 0 || bposition.x >= CHUNK_SIZE || bposition.y < 0 ||
            bposition.y >= CHUNK_SIZE || bposition.z < 0 ||
            bposition.z >= CHUNK_SIZE) {
            return BlockType::Air;
        }
        else {
            return m_blocks[(bposition.y * (CHUNK_SIZE * CHUNK_SIZE) +
                             bposition.z * CHUNK_SIZE + bposition.x)];
        }
    }*/