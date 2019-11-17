#include "chunk.h"

#include <iostream>

#include <common/coordinate_convertion.h>

Chunk::Chunk(int x, int y, int z)
    : position({x, y, z})
{
}
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

sf::Packet &operator<<(sf::Packet &packet, const Chunk &chunk)
{
    packet << chunk.position.x << chunk.position.y << chunk.position.z;
    for (auto block : chunk.m_blocks) {
        packet << static_cast<u8>(block.type);
    }
    return packet;
}

sf::Packet &operator>>(sf::Packet &packet, Chunk &chunk)
{
    for (auto &block : chunk.m_blocks) {
        u8 blockId;
        packet >> blockId;
        block = static_cast<BlockType>(blockId);
    }
    return packet;
}
