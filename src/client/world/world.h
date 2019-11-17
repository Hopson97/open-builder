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

        void addChunk(const ChunkPosition &position, Chunk &&chunk);
        void removeChunk(const ChunkPosition &position);

        const ChunkPositionMap<ChunkMesh>& getChunkMeshes() const;

        EntityArray entities;

      private:
        ChunkPositionMap<Chunk> m_chunks;
        ChunkPositionMap<ChunkState> m_chunkStates;
        ChunkPositionMap<ChunkMesh> m_chunkMeshes;
    };
} // namespace client
