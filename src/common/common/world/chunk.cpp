#include "chunk.h"
#include "chunk_manager.h"

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
