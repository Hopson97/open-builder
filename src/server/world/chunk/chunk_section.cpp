#include "chunk_section.h"

#include <iostream>

#include <common/coordinate_convertion.h>

namespace server {
    ChunkSection::ChunkSection(int x, int y, int z)
        : position({x, y, z})
    {
    }
    ChunkSection::ChunkSection(const ChunkSectionPosition &chunkPosition)
        : position(chunkPosition)
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

    sf::Packet &operator<<(sf::Packet &packet, const ChunkSection &chunk)
    {
        packet << chunk.position.x << chunk.position.y << chunk.position.z;
        for (auto block : chunk.m_blocks) {
            packet << static_cast<u8>(block.type);
        }
        return packet;
    }
} // namespace server
