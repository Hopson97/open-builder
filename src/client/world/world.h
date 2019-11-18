#pragma once

#include "chunk/chunk.h"
#include "chunk/mesh/chunk_mesh.h"
#include "entity.h"

namespace client {
    struct World {
      public:
        World();
        void update(Entity &player);

        EntityArray entities;

      //private:
        std::vector<Chunk> m_chunks;
    };
} // namespace client
