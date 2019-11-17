#include "chunk.h"

#include <iostream>

#include <common/coordinate_convertion.h>

Chunk::Chunk(const ChunkPosition &chunkPosition)
    : position(chunkPosition)
{
}

Block Chunk::getBlock(const BlockPosition &blockPosition) const
{
    if (positionOutOfChunkBounds(blockPosition)) {
        return BlockType::Air;
    }
    else {
        return m_blocks[toChunkBlockIndex(blockPosition)];
    }
}

void Chunk::setBlock(const BlockPosition &blockPosition, Block block)
{
    if (!positionOutOfChunkBounds(blockPosition)) {
        m_blocks[toChunkBlockIndex(blockPosition)] = block;
    }
}

void Chunk::quickSetBlock(const BlockPosition &blockPosition, Block block)
{
    m_blocks[toChunkBlockIndex(blockPosition)] = block;
}
