#pragma once

#include "chunk_section.h"
#include "mesh/chunk_mesh.h"

namespace client {
    class Renderer;
    class Chunk final {
      public:
        Chunk(const ChunkPosition &chunkPosition, World &world);

        void addSection(ChunkSection section);
        Block getBlock(const BlockPosition &blockPosition);

        void render(Renderer &renderer);
        bool createMesh();

        int countSections() const;
        const ChunkPosition& getPosition() const;

      private:
        ChunkPosition m_position;
        std::vector<ChunkSection> m_sections;
        std::vector<ChunkMesh> m_chunkMeshes;

        World &mp_world;
    };
} // namespace client
