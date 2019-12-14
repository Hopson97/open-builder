#include "chunk.h"

#include "../debug.h"
#include <iostream>

Chunk::Chunk(ChunkManager *manager)
    : mp_manager(manager)
{
}

u8 Chunk::qGetBlock(const BlockPosition &blockPosition) const
{
    return blocks.at(toLocalBlockIndex(blockPosition));
}

void Chunk::qSetBlock(const BlockPosition &blockPosition, u8 block)
{
    blocks.at(toLocalBlockIndex(blockPosition)) = block;
}

Chunk &ChunkManager::addChunk(const ChunkPosition &chunk)
{
    auto itr = m_chunks.find(chunk);
    if (itr == m_chunks.cend()) {
        return m_chunks.emplace(chunk, Chunk{this}).first->second;
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
    return itr->second.qGetBlock(toLocalBlockPosition(blockPosition));
}

void ChunkManager::setBlock(const BlockPosition &blockPosition, u8 block)
{
    auto chunkPosition = toChunkPosition(blockPosition);
    auto itr = m_chunks.find(chunkPosition);
    if (itr != m_chunks.cend()) {
        itr->second.qSetBlock(toLocalBlockPosition(blockPosition), block);
    }
}

bool ChunkManager::hasChunk(const ChunkPosition &chunk) const
{
    return m_chunks.find(chunk) != m_chunks.cend();
}

bool ChunkManager::hasNeighbours(const ChunkPosition &chunkPosition) const
{
    const auto& cp = chunkPosition;
    return hasChunk(chunkPosition) &&
           // Top
           hasChunk({cp.x, cp.y + 1, cp.z}) &&
           // Bottom
           hasChunk({cp.x, cp.y - 1, cp.z}) &&
           // Left
           hasChunk({cp.x - 1, cp.y, cp.z}) &&
           // Right
           hasChunk({cp.x + 1, cp.y, cp.z}) &&
           // Front
           hasChunk({cp.x, cp.y, cp.z - 1}) &&
           // Back
           hasChunk({cp.x, cp.y, cp.z + 1});
}
