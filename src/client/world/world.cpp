#include "world.h"

#include "chunk/mesh/chunk_mesh_builder.h"

namespace client {
    World::World()
    {
        ChunkPosition position(0, 0, 0);
        Chunk chunk(position);
        chunk.flag = Chunk::Flag::NeedsNewMesh;

        chunks.insert(std::make_pair(position, chunk));
    }

    void World::update()
    {
        for (auto &[position, chunk] : chunks) {
            if (chunk.flag == Chunk::Flag::NeedsNewMesh) {
                ChunkMeshBuilder builder(chunk);
                chunkMeshes[position] = builder.createMesh();
                chunk.flag = Chunk::Flag::None;
            }
        }
    }
} // namespace client
