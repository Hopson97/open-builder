#include "world.h"

#include "chunk/mesh/chunk_mesh_builder.h"

#include <iostream>

namespace client {
    World::World()
    {
        for (int z = 0; z < WORLD_SIZE; ++z) {
            for (int x = 0; x < WORLD_SIZE; ++x) {
                m_chunks.emplace_back(x, z);
            }
        }
    }

    void World::addChunk(ChunkSection&& section)
    {
        m_chunks.at(section.position.z * WORLD_SIZE + section.position.x).addSection(std::move(section));
    }

    void World::render(Renderer& renderer)
    {
        for (auto& chunk : m_chunks) {
            chunk.render(renderer);
        }
    }

    void World::update([[maybe_unused]] Entity &player)
    {
        for (auto& chunk : m_chunks) {
            if (chunk.createMesh()) {
                break;
            }
        }
        /*
                for (auto &[position, chunk] : chunks) {
                    if (chunk.flag == ChunkSection::Flag::NeedsNewMesh) {
                        ChunkMeshBuilder builder(chunk);
                        chunkMeshes[position] = builder.createMesh();
                        chunk.flag = ChunkSection::Flag::None;
                        return;
                    }
                }
        */
    }
} // namespace client

/*
static int distance = 1;
bool meshMade = false;
[&]() {
    for (int z = -distance; z <= distance; z++) {
        for (int x = -distance; x <= distance; x++) {
            int chunkX = (player.transform.position.x) / CHUNK_SIZE + x;
            int chunkZ = (player.transform.position.z) / CHUNK_SIZE + z;
            auto itr = chunks.find({chunkX, 0, chunkZ});
            if (itr != chunks.cend()) {
                auto &chunk = itr->second;
                if (chunk.flag == ChunkSection::Flag::NeedsNewMesh) {
                    ChunkMeshBuilder builder(chunk);
                    chunkMeshes[itr->first] = builder.createMesh();
                    chunk.flag = ChunkSection::Flag::None;
                    meshMade = true;
                    return;
                }
            }
        }
    }
}();

std::cout << distance << std::endl;
if (!meshMade) {
    distance++;
}
*/