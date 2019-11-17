#include "world.h"

#include "chunk/chunk_mesh_builder.h"

#include <iostream>

namespace client {
    World::World()
    {
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

        for (auto &[position, chunk] : chunks) {
            if (chunk.flag == ClientChunk::Flag::NeedsNewMesh) {
                ChunkMeshBuilder builder(chunk);
                chunkMeshes[position] = builder.createMesh();
                chunk.flag = ClientChunk::Flag::None;
                return;
            }
        }
    }
} // namespace client
