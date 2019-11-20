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

        void render(Renderer &renderer);

        Block getBlock(const BlockPosition &blockPosition);
        Chunk &getChunk(const ChunkPosition &position);

      private:
        std::vector<Chunk> m_chunks;

        Chunk m_emptyChunk;
    };
} // namespace client
