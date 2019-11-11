#pragma once 

#include "entity.h"
#include "chunk/chunk.h"
#include "chunk/mesh/chunk_mesh.h"

namespace client
{
    struct World
    {
        public:
            World();
            void update();

            EntityArray entities;
            ChunkPositionMap<Chunk> chunks;
            ChunkPositionMap<ChunkMesh> chunkMeshes;
    };  
} // namespace client
