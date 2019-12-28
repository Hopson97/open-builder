#include "chunk.h"
#include "chunk_manager.h"

// TODO Replace the .at with operator[] (when can be sure its safe to do so)

Chunk::Chunk(ChunkManager &manager, const ChunkPosition &position)
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
        return mp_manager.getBlock(
            toGlobalBlockPosition(blockPosition, m_position));
    }
    return qGetBlock(blockPosition);
}

const ChunkPosition &Chunk::getPosition() const
{
    return m_position;
}

Chunk::CompressedBlocks Chunk::compress() const
{
    CompressedBlocks comp;

    block_t current = blocks[0];
    u32 count = 1;

    for (unsigned i = 1; i < blocks.size(); i++) {
        if (blocks[i] == current) {
            count++;
        }
        else {
            comp.emplace_back(current, count);
            current = blocks[i];
            count = 1;
        }
    }
    comp.emplace_back(current, count);
    return comp;
}

void Chunk::decompress(const Chunk::CompressedBlocks &comp)
{
    int pointer = 0;
    for (auto &block : comp) {
        block_t type = block.first;
        u32 count = block.second;

        for (u32 i = 0; i < count; i++) {
            blocks[pointer++] = type;
        }
    }
}