#include "chunk.h"

namespace server {
    Chunk::Chunk(int x, int z)
    :   m_position(x, z)
    {
    }

    void Chunk::addSection(int chunkY)
    {
        while (chunkY > m_sections.size() - 1) 
        {
            m_sections.emplace_back(m_position.x, m_sections.size(), m_position.y);
        }
    }

    void Chunk::setBlock(int x, int y, int z, Block block)
    {
        int blockY = y % CHUNK_SIZE;
        int sectionIndex = y / CHUNK_SIZE;
        if (sectionIndex >=  m_sections.size())
        {
            addSection(sectionIndex);
        }

        m_sections[sectionIndex].quickSetBlock({x, blockY, z}, block);
    }

    Block Chunk::getBlock(int x, int y, int z)
    {
        int blockY = y % CHUNK_SIZE;
        int sectionIndex = y / CHUNK_SIZE;
        if (sectionIndex >=  m_sections.size())
        {
            addSection(sectionIndex);
        }

        m_sections[sectionIndex].getBlock({x, blockY, z});
    }

        void Chunk::generateTerrain()
        {

        }

        bool Chunk::readyForSend() const
        {

        }

        std::vector<Packet> Chunk::createPackets() const
        {
            
        }
} // namespace server
