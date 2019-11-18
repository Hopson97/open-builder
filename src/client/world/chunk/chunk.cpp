#include "chunk.h"

#include "../../renderer/renderer.h"
#include "mesh/chunk_mesh_builder.h"

#include <iostream>

namespace client {
    Chunk::Chunk(int x, int z, World& world)
        : m_position(x, z)
        , mp_world(world)
    {
    }

    void Chunk::addSection(ChunkSection section)
    {
        if (section.getPosition().y >= 0) {
            while (section.getPosition().y > (int)m_sections.size() - 1) {
                m_sections.emplace_back(m_position.x, m_sections.size(),
                                        m_position.y, mp_world);
                m_chunkMeshes.emplace_back();
            }
            m_sections[section.getPosition().y] = section;
        }
    }

    Block Chunk::getBlock(int x, int y, int z)
    {
        if (y < 0) {
            return BlockType::Air;
        }

        int blockY = y % CHUNK_SIZE;
        unsigned sectionIndex = y / CHUNK_SIZE;

        if (sectionIndex >= m_sections.size()) {
            return BlockType::Air;
        }

        return m_sections[sectionIndex].getBlock({x, blockY, z});
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
            if (!m_chunkMeshes[i].solidBlocks.isCreated()) {
                ChunkMeshBuilder builder(m_sections[i]);
                auto mesh = builder.createMesh();
                m_chunkMeshes[i] = std::move(mesh);
                return true;
            }
        }
        return false;
    }

} // namespace client
