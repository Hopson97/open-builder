#include "chunk.h"

#include "../world.h"
#include <common/coordinate_convertion.h>
#include <iostream>

#include <common/debug.h>

namespace client {
    ChunkSection::ChunkSection(int x, int y, int z, World &world)
        : mp_world(&world)
        , m_position(x, y, z)
    {
    }

    Block ChunkSection::getBlock(const BlockPosition &blockPosition) const
    {
        if (positionOutOfChunkBounds(blockPosition)) {

            auto location =
                localBlockToWorldBlockPostion(blockPosition, m_position);

            return mp_world->getBlock(location);
        }
        else {
            return m_blocks[toChunkBlockIndex(blockPosition)];
        }
    }

    const ChunkSectionPosition &ChunkSection::getPosition() const
    {
        return m_position;
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
