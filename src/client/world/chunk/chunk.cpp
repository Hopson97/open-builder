#include "chunk.h"

#include <iostream>

namespace client {
    Chunk::Chunk()
    {
    }

    Block Chunk::getBlock(const LocalBlockPosition &position) const
    {
        if (position.x < 0 || position.x >= SIZE || position.y < 0 ||
            position.y >= SIZE || position.z < 0 || position.z >= SIZE) {
            return BlockType::Air;
        }
        else {
            return m_blocks[(position.y * (SIZE * SIZE) + position.z * SIZE +
                             position.x)];
        }
    }
} // namespace client
