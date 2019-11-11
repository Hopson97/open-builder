#include "chunk.h"

#include <iostream>

namespace client {
    Chunk::Chunk()
    {
    }

    Block Chunk::getBlock(const LocalBlockPosition &position) const
    {
        if (position.x < 0 || position.x >= CHUNK_SIZE || position.y < 0 ||
            position.y >= CHUNK_SIZE || position.z < 0 || position.z >= CHUNK_SIZE) {
            return BlockType::Air;
        }
        else {
            return m_blocks[(position.y * (CHUNK_SIZE * CHUNK_SIZE) + position.z * CHUNK_SIZE +
                             position.x)];
        }
    }
} // namespace client
