#include "chunk_manager.h"

Chunk &ChunkManager::addChunk(const ChunkPosition &chunk)
{
    auto itr = m_chunks.find(chunk);
    if (itr == m_chunks.cend()) {
        return m_chunks
            .emplace(std::piecewise_construct, std::forward_as_tuple(chunk),
                     std::forward_as_tuple(this, chunk))
            .first->second;
    }
    return itr->second;
}

const Chunk &ChunkManager::getChunk(const ChunkPosition &chunk)
{
    auto itr = m_chunks.find(chunk);
    if (itr == m_chunks.cend()) {
        static Chunk errorChunk(this, {0, 0, 0});
        return errorChunk;
    }
    return itr->second;
}

block_t ChunkManager::getBlock(const BlockPosition &blockPosition) const
{
    auto chunkPosition = toChunkPosition(blockPosition);
    auto itr = m_chunks.find(chunkPosition);
    if (itr == m_chunks.cend()) {
        return 0;
    }
    return itr->second.qGetBlock(toLocalBlockPosition(blockPosition));
}

void ChunkManager::setBlock(const BlockPosition &blockPosition, block_t block)
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
    const auto &cp = chunkPosition;
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