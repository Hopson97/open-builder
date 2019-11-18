#include "chunk.h"

#include "../../network/server.h"

namespace server {
    Chunk::Chunk(int x, int z)
        : m_position(x, z)
    {
    }

    void Chunk::addSection(int chunkY)
    {
        if (chunkY >= 0) {
            while (static_cast<unsigned>(chunkY) > m_sections.size() - 1) {
                m_sections.emplace_back(m_position.x, m_sections.size(),
                                        m_position.y);
            }
        }
    }

    void Chunk::setBlock(int x, int y, int z, Block block)
    {
        if (y < 0) {
            return;
        }

        int blockY = y % CHUNK_SIZE;
        unsigned sectionIndex = y / CHUNK_SIZE;

        if (sectionIndex >= m_sections.size()) {
            addSection(sectionIndex);
        }

        m_sections[sectionIndex].quickSetBlock({x, blockY, z}, block);
    }

    Block Chunk::getBlock(int x, int y, int z)
    {
        if (y < 0) {
            return BlockType::Air;
        }

        int blockY = y % CHUNK_SIZE;
        unsigned sectionIndex = y / CHUNK_SIZE;

        if (sectionIndex >= m_sections.size()) {
            addSection(sectionIndex);
        }

        return m_sections[sectionIndex].getBlock({x, blockY, z});
    }

    void Chunk::generateTerrain()
    {
        for (int y = 0; y < CHUNK_SIZE + 1; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                for (int x = 0; x < CHUNK_SIZE; x++) {
                    setBlock(x, y, z, BlockType::Grass);
                }
            }
        }
    }

    bool Chunk::readyForSend() const
    {
        return m_sections.size() > 0;
    }

    void Chunk::sendChunks(Server& server) const
    {
        for (auto& chunk : m_sections)
        {
            auto packet = server.createPacket(CommandToClient::ChunkData, Packet::Flag::Reliable);
            packet.payload << chunk;
            server.sendToAllClients(packet); 
        }
    }
} // namespace server
