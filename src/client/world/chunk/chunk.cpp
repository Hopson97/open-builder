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

    Block Chunk::getBlock(const BlockPosition &blockPosition) const
    {
        if (position.x < 0 || position.x >= CHUNK_SIZE || position.y < 0 ||
           position.y >= CHUNK_SIZE || position.z < 0 ||
           position.z >= CHUNK_SIZE) {
            std::cout << "Out of boundd!" << std::endl;
            return BlockType::Air;
        }
        else {
            return BlockType::Grass;
            return m_blocks[toChunkBlockIndex(blockPosition)];
        }
    }

    sf::Packet& operator>>(sf::Packet& packet, Chunk& chunk)
    {
        std::cout << "Data recieved\n";
        for (auto& block : chunk.m_blocks)
        {
            u8 blockId;
            packet >> blockId;
            block = static_cast<BlockType>(blockId);
        }
        return packet;
    }
} // namespace client
