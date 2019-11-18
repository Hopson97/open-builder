#include "chunk_section.h"

#include <iostream>

#include <common/coordinate_convertion.h>

namespace client {
    ChunkSection::ChunkSection(int x, int y, int z)
        : position({x, y, z})
    {
    }

    ChunkSection::ChunkSection(const ChunkPosition &chunkPosition)
        : position(chunkPosition)
    {
    }

    Block ChunkSection::getBlock(const BlockPosition &blockPosition) const
    {
        if (positionOutOfChunkBounds(blockPosition)) {
            return BlockType::Air;
        }
        else {
            return m_blocks[toChunkBlockIndex(blockPosition)];
        }
    }

    sf::Packet &operator>>(sf::Packet &packet, ChunkSection &chunk)
    {
        for (auto &block : chunk.m_blocks) {
            u8 blockId;
            packet >> blockId;
            block = static_cast<BlockType>(blockId);
        }
        return packet;
    }
} // namespace client
