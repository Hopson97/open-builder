#include "chunk.h"

#include <iostream>

#include <common/coordinate_convertion.h>

namespace server {
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

    void Chunk::setBlock(const BlockPosition &position, Block block)
    {
        if (!positionOutOfChunkBounds(position)) {
            m_blocks[toChunkBlockIndex(position)] = block;
        }
    }

    void Chunk::quickSetBlock(const BlockPosition &position, Block block)
    {
        m_blocks[toChunkBlockIndex(position)] = block;
    }
} // namespace client
