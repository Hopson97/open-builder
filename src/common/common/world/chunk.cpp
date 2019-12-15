#include "chunk.h"

#include "../debug.h"
#include <iostream>

Chunk::Chunk(ChunkManager *manager, const ChunkPosition &position)
    : mp_manager(manager)
    , m_position(position)
{
}

block_t Chunk::qGetBlock(const BlockPosition &blockPosition) const
{
    return blocks.at(toLocalBlockIndex(blockPosition));
}

void Chunk::qSetBlock(const BlockPosition &blockPosition, block_t block)
{
    blocks.at(toLocalBlockIndex(blockPosition)) = block;
}

block_t Chunk::getBlock(const BlockPosition &blockPosition) const
{
    if (blockPosition.x < 0 || blockPosition.x >= CHUNK_SIZE ||
        blockPosition.y < 0 || blockPosition.y >= CHUNK_SIZE ||
        blockPosition.z < 0 || blockPosition.z >= CHUNK_SIZE) {
        return mp_manager->getBlock(
            toGlobalBlockPosition(blockPosition, m_position));
    }
    return qGetBlock(blockPosition);
}

const ChunkPosition &Chunk::getPosition() const
{
    return m_position;
}

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

Chunk &ChunkManager::addChunk(Chunk &&chunk)
{
    auto itr = m_chunks.find(chunk.getPosition());
    if (itr == m_chunks.cend()) {
        return m_chunks.emplace(chunk.getPosition(), std::move(chunk)).first->second;
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
