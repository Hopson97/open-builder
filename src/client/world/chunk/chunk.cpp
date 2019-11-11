#include "chunk.h"

namespace client {
    Chunk::Chunk()
    {
    }

    Block Chunk::getBlock(const LocalBlockPosition &position) const
    {
        if (position.x < 0 || position.x >= 32 || position.y < 0 ||
            position.y >= 32 || position.z < 0 || position.x >= 32) {
            return BlockType::Grass;
        }
        else {
            return m_blocks[position.y * (32 * 32) * position.z * 32 +
                            position.x];
        }
    }
} // namespace client
