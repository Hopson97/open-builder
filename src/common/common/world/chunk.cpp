#include "chunk.h"

#include <iostream>

#include <common/coordinate_convertion.h>

ChunkTEMP::ChunkTEMP(const ChunkPosition &chunkPosition)
    : position(chunkPosition)
{
}

Block ChunkTEMP::getBlock(const BlockPosition &blockPosition) const
{
    if (positionOutOfChunkBounds(blockPosition)) {
        return BlockType::Air;
    }
    else {
        return m_blocks[toChunkBlockIndex(blockPosition)];
    }
}

void ChunkTEMP::setBlock(const BlockPosition &blockPosition, Block block)
{
    if (!positionOutOfChunkBounds(blockPosition)) {
        m_blocks[toChunkBlockIndex(blockPosition)] = block;
    }
}

void ChunkTEMP::quickSetBlock(const BlockPosition &blockPosition, Block block)
{
    m_blocks[toChunkBlockIndex(blockPosition)] = block;
}
