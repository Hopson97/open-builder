#include "chunk.h"

#include <common/coordinate.h>
#include <iostream>

namespace server {
    ChunkSection::ChunkSection(const ChunkSectionPosition &chunkPosition)
        : m_position(chunkPosition)
    {
    }

    Block ChunkSection::getBlock(const BlockPosition &BlockPosition) const
    {
        if (positionOutOfChunkBounds(BlockPosition)) {
            return BlockType::Air;
        }
        else {
            return m_blocks[toChunkBlockIndex(BlockPosition)];
        }
    }

    void ChunkSection::setBlock(const BlockPosition &BlockPosition, Block block)
    {
        if (!positionOutOfChunkBounds(BlockPosition)) {
            m_blocks[toChunkBlockIndex(BlockPosition)] = block;
        }
    }

    void ChunkSection::quickSetBlock(const BlockPosition &BlockPosition,
                                     Block block)
    {
        m_blocks[toChunkBlockIndex(BlockPosition)] = block;
    }

    const ChunkSectionPosition &ChunkSection::getPosition() const
    {
        return m_position;
    }

    sf::Packet &operator<<(sf::Packet &packet, const ChunkSection &chunk)
    {
        packet << chunk.getPosition().x << chunk.getPosition().y
               << chunk.getPosition().z;
        for (auto block : chunk.m_blocks) {
            packet << static_cast<u8>(block.type);
        }
        return packet;
    }
} // namespace server
