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

CompressedBlocks compressBlockData(const BlockArray &blocks)
{
    CompressedBlocks compressed;
    block_t currentBlock = blocks[0];
    u32 blockCount = 1;

    for (unsigned i = 1; i < blocks.size(); i++) {
        auto block = blocks[i];
        if (block == currentBlock) {
            blockCount++;
        }
        else {
            compressed.emplace_back(currentBlock, blockCount);
            currentBlock = blocks[i];
            blockCount = 1;
        }
    }
    compressed.emplace_back(currentBlock, blockCount);
    return compressed;
}

BlockArray decompressBlockData(const CompressedBlocks &blocks)
{
    BlockArray blockData;
    int blockPointer = 0;
    for (auto &block : blocks) {
        auto type = block.first;
        auto count = block.second;

        for (u16 i = 0; i < count; i++) {
            blockData[blockPointer++] = type;
        }
    }
    return blockData;
}
