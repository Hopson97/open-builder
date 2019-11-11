#include "chunk.h"

#include <iostream>

#include <common/coordinate_convertion.h>

namespace client {
    Chunk::Chunk()
    {
    }

    Block Chunk::getBlock(const BlockPosition &position) const
    {
        if (positionOutOfChunkBounds(position)) {
            return BlockType::Air;
        }
        else {
            return m_blocks[toChunkBlockIndex(position)];
        }
    }
} // namespace client
