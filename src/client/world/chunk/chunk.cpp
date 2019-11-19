#include "chunk.h"

#include "../../renderer/renderer.h"
#include "../world.h"

#include <iostream>

namespace client {
    Chunk::Chunk(const ChunkPosition &chunkPosition, World &world)
        : m_position(chunkPosition)
        , mp_world(world)
    {
    }

    void Chunk::addSection(ChunkSection section)
    {
        if (section.getPosition().y >= 0) {
            while (section.getPosition().y > (int)m_sections.size() - 1) {
                m_sections.emplace_back(m_position.x, m_sections.size(),
                                        m_position.z, mp_world);
                m_chunkMeshes.emplace_back();
            }
            m_sections[section.getPosition().y] = section;
        }
    }

    Block Chunk::getBlock(const BlockPosition &blockPosition)
    {
        if (blockPosition.y < 0) {
            return BlockType::Air;
        }

        int blockY = blockPosition.y % CHUNK_SIZE;
        unsigned sectionIndex = blockPosition.y / CHUNK_SIZE;

        if (sectionIndex >= m_sections.size()) {
            return BlockType::Air;
        }

        return m_sections[sectionIndex].getBlock(
            {blockPosition.x, blockY, blockPosition.z});
    }

    void Chunk::render(Renderer &renderer)
    {
        for (auto &mesh : m_chunkMeshes) {
            renderer.process(mesh);
        }
    }

    bool Chunk::createMesh()
    {
        for (unsigned i = 0; i < m_sections.size(); i++) {
            if (!m_chunkMeshes[i].solid.isCreated()) {
                auto meshGroup = createChunkMesh(m_sections[i]);
                m_chunkMeshes[i] = bufferChunkMeshGroup(meshGroup);
                return true;
            }
        }
        return false;
    }

    int Chunk::countSections() const
    {
        return m_sections.size();
    }

    const ChunkPosition &Chunk::getPosition() const
    {
        return m_position;
    }

} // namespace client
