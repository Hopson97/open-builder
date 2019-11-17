#include "world.h"

#include "chunk/chunk_mesh_builder.h"

#include <iostream>

namespace client {
    World::World()
    {
    }

    void World::addChunk(const ChunkPosition &position, Chunk &&chunk)
    {
        m_chunks.emplace(position, std::move(chunk));
        m_chunkStates.emplace(position, ChunkState::Flag::NeedsNewMesh);
    }

    void World::removeChunk(const ChunkPosition &position)
    {
        m_chunks.erase(position);
        m_chunkStates.erase(position);
        m_chunkMeshes.erase(position);
    }

    const ChunkPositionMap<ChunkMesh>& World::getChunkMeshes() const
    {
        return m_chunkMeshes;
    }

    void World::update([[maybe_unused]] Entity &player)
    {
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
                        if (chunk.flag == Chunk::Flag::NeedsNewMesh) {
                            ChunkMeshBuilder builder(chunk);
                            chunkMeshes[itr->first] = builder.createMesh();
                            chunk.flag = Chunk::Flag::None;
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

        for (auto &[position, chunk] : m_chunkStates) {
            if (chunk.flags == (u8)ChunkState::Flag::NeedsNewMesh) {
                ChunkMeshBuilder builder(m_chunks.at(position));
                m_chunkMeshes[position] = builder.createMesh();
                chunk.flags = (u8)ChunkState::Flag::None;
                return;
            }
        }
    }
} // namespace client
