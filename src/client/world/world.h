#pragma once

#include "chunk/chunk.h"
#include "chunk/mesh/chunk_mesh.h"
#include "entity.h"

namespace client {
    struct World {
      public:
        World();
        void update();

        EntityArray entities;
        ChunkPositionMap<Chunk> chunks;
        ChunkPositionMap<ChunkMesh> chunkMeshes;
    };
} // namespace client
