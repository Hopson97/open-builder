#include "chunk.h"

#include <iostream>

#include <common/coordinate_convertion.h>

Chunk::Chunk(const ChunkPosition &chunkPosition)
    : position(chunkPosition)
{
}

Block Chunk::getBlock(const BlockPosition &blockPosition) const
{
    if (blockPosition.x < 0) return BlockType::Air;
    else if (blockPosition.x >= CHUNK_SIZE) return BlockType::Air;
    if (blockPosition.y < 0) return BlockType::Air;
    else if (blockPosition.y >= CHUNK_SIZE) return BlockType::Air;
    if (blockPosition.z < 0) return BlockType::Air;
    else if (blockPosition.z >= CHUNK_SIZE) return BlockType::Air;
    
    return m_blocks[toChunkBlockIndex(blockPosition)];
    
}

void Chunk::setBlock(const BlockPosition &blockPosition, Block block)
{
    if (blockPosition.x < 0) return;
    else if (blockPosition.x >= CHUNK_SIZE) return;
    if (blockPosition.y < 0) return;
    else if (blockPosition.y >= CHUNK_SIZE) return;
    if (blockPosition.z < 0) return;
    else if (blockPosition.z >= CHUNK_SIZE) return;

    m_blocks[toChunkBlockIndex(blockPosition)] = block;
}

void Chunk::quickSetBlock(const BlockPosition &blockPosition, Block block)
{
    m_blocks[toChunkBlockIndex(blockPosition)] = block;
}
