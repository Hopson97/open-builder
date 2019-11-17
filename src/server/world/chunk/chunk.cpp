#include "chunk.h"

#include <iostream>

#include <common/coordinate_convertion.h>

namespace server {
    ServerChunk::ServerChunk(const ChunkPosition &chunkPosition)
        : Chunk(chunkPosition)
    {
    }

    sf::Packet &operator<<(sf::Packet &packet, const ServerChunk &chunk)
    {
        packet << chunk.position.x << chunk.position.y << chunk.position.z;
        for (auto block : chunk.m_blocks) {
            packet << static_cast<u8>(block.type);
        }
        return packet;
    }
} // namespace server
