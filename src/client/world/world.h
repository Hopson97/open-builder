#pragma once

#include "chunk/chunk.h"
#include "chunk/chunk_mesh.h"
#include "entity.h"

namespace client {
    struct World {
      public:
        World();
        void update(Entity &player);

        EntityArray entities;
        ChunkPositionMap<ClientChunk> chunks;
        ChunkPositionMap<ChunkMesh> chunkMeshes;
    };
} // namespace client
