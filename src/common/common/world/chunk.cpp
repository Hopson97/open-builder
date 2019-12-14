#include "chunk.h"

#include <iostream>
#include "../debug.h"

u8 Chunk::getBlock(const BlockPosition &blockPosition) const
{
    return blocks.at(toLocalBlockIndex(blockPosition));
}

void Chunk::setBlock(const BlockPosition &blockPosition, u8 block)
{
    blocks.at(toLocalBlockIndex(blockPosition)) = block;
}

Chunk &ChunkManager::addChunk(const ChunkPosition &chunk)
{
    auto itr = m_chunks.find(chunk);
    if (itr == m_chunks.cend()) {
        return m_chunks.emplace(chunk, Chunk{}).first->second;
    }
    return itr->second;
}

u8 ChunkManager::getBlock(const BlockPosition &blockPosition) const
{
    auto chunkPosition = toChunkPosition(blockPosition);
    auto itr = m_chunks.find(chunkPosition);
    if (itr == m_chunks.cend()) {
        return 0;
    }
    return itr->second.getBlock(toLocalBlockPosition(blockPosition));
}

void ChunkManager::setBlock(const BlockPosition &blockPosition, u8 block)
{
    auto chunkPosition = toChunkPosition(blockPosition);
    auto itr = m_chunks.find(chunkPosition);
    if (itr != m_chunks.cend()) {
        itr->second.setBlock(toLocalBlockPosition(blockPosition), block);
    }
}
