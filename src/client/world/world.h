#pragma once

#include "chunk/chunk_mesh.h"
#include "chunk/chunk_state.h"
#include "entity.h"

#include <common/world/chunk.h>

namespace client {
    struct World {
      public:
        World();
        void update(Entity &player);

        EntityArray entities;
        ChunkPositionMap<Chunk> chunks;
        ChunkPositionMap<ChunkState> chunkStates;
        ChunkPositionMap<ChunkMesh> chunkMeshes;
    };
} // namespace client
