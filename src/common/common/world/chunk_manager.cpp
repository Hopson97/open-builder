#include "chunk_manager.h"

#include <iostream>

Chunk& ChunkManager::addChunk(const ChunkPosition& chunk)
{
    auto itr = m_chunks.find(chunk);
    if (itr == m_chunks.cend()) {
        return m_chunks
            .emplace(std::piecewise_construct, std::forward_as_tuple(chunk),
                     std::forward_as_tuple(*this, chunk))
            .first->second;
    }
    return itr->second;
}

const Chunk& ChunkManager::getChunk(const ChunkPosition& chunk)
{
    auto itr = m_chunks.find(chunk);
    if (itr == m_chunks.cend()) {
        static Chunk errorChunk(*this, {0, 0, 0});
        return errorChunk;
    }
    return itr->second;
}

voxel_t ChunkManager::getVoxel(const VoxelPosition& voxelPosition) const
{
    auto chunkPosition = toChunkPosition(voxelPosition);
    auto itr = m_chunks.find(chunkPosition);
    if (itr == m_chunks.cend()) {
        return 0;
    }
    return itr->second.qGetVoxel(toLocalVoxelPosition(voxelPosition));
}

void ChunkManager::setVoxel(const VoxelPosition& voxelPosition, voxel_t voxel)
{
    auto chunkPosition = toChunkPosition(voxelPosition);
    auto itr = m_chunks.find(chunkPosition);
    auto local = toLocalVoxelPosition(voxelPosition);
    if (itr != m_chunks.cend()) {
        itr->second.qSetVoxel(local, voxel);
    }
    else {
        addChunk(chunkPosition).qSetVoxel(local, voxel);
    }
    ensureNeighbours(chunkPosition);
}

bool ChunkManager::hasChunk(const ChunkPosition& chunk) const
{
    return m_chunks.find(chunk) != m_chunks.cend();
}

bool ChunkManager::hasNeighbours(const ChunkPosition& chunkPosition) const
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

void ChunkManager::ensureNeighbours(const ChunkPosition& chunkPosition)
{
    const auto& cp = chunkPosition;
    addChunk(cp);
    addChunk({cp.x, cp.y + 1, cp.z});
    addChunk({cp.x, cp.y - 1, cp.z});
    addChunk({cp.x - 1, cp.y, cp.z});
    addChunk({cp.x + 1, cp.y, cp.z});
    addChunk({cp.x, cp.y, cp.z - 1});
    addChunk({cp.x, cp.y, cp.z + 1});
}

const ChunkPositionMap<Chunk>& ChunkManager::chunks() const
{
    return m_chunks;
}
