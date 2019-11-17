#pragma once

#include "chunk/chunk_mesh.h"
#include "chunk/chunk_state.h"
#include "entity.h"

#include <common/world/chunk.h>

#include <mutex>
#include <thread>

namespace client {
    struct World {
      public:
        World();
        void update(Entity &player);

        void addChunk(const ChunkPosition &position, Chunk &&chunk);
        void removeChunk(const ChunkPosition &position);

        const ChunkPositionMap<ChunkDrawable> &getChunkDrawables() const;

        EntityArray entities;

      private:
        ChunkPositionMap<Chunk> m_chunks;
        ChunkPositionMap<ChunkState> m_chunkStates;
        ChunkPositionMap<ChunkMesh> m_chunkMeshes;
        ChunkPositionMap<ChunkDrawable> m_drawableChunks;

        std::vector<std::thread> m_workers;
        std::mutex m_chunkMeshBuilderMutex;
    };
} // namespace client
