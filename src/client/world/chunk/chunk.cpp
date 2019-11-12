#include "chunk.h"

#include <iostream>

#include <common/coordinate_convertion.h>

namespace client {
    Chunk::Chunk(int x, int y, int z)
        : position({x, y, z})
    {
    }

    Chunk::Chunk(const ChunkPosition &chunkPosition)
        : position(chunkPosition)
    {
    }

    Block Chunk::getBlock(const BlockPosition &bposition) const
    {
        if (bposition.x < 0 || bposition.x >= CHUNK_SIZE || bposition.y < 0 ||
            bposition.y >= CHUNK_SIZE || bposition.z < 0 ||
            bposition.z >= CHUNK_SIZE) {
            return BlockType::Air;
        }
        else {
            return m_blocks[(bposition.y * (CHUNK_SIZE * CHUNK_SIZE) +
                             bposition.z * CHUNK_SIZE + bposition.x)];
        }
    }

    sf::Packet &operator>>(sf::Packet &packet, Chunk &chunk)
    {
        std::cout << "Data recieved\n";
        for (auto &block : chunk.m_blocks) {
            u8 blockId;
            packet >> blockId;
            block = static_cast<BlockType>(blockId);
        }
        return packet;
    }
} // namespace client
