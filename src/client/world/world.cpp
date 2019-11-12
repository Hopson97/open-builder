#include "world.h"

#include "chunk/mesh/chunk_mesh_builder.h"

namespace client {
    World::World()
    {
    }

    void World::update()
    {
        for (auto &[position, chunk] : chunks) {
            if (chunk.flag == Chunk::Flag::NeedsNewMesh) {
                ChunkMeshBuilder builder(chunk);
                chunkMeshes[position] = builder.createMesh();
                chunk.flag = Chunk::Flag::None;
                return;
            }
        }
    }
} // namespace client
