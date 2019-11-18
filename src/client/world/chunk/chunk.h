#pragma once

#include "chunk_section.h"
#include "mesh/chunk_mesh.h"
#include <SFML/System/Vector2.hpp>

namespace client {
    class Renderer;
    class Chunk final {
      public:
        Chunk(int x, int z);

        void addSection(ChunkSection&& section);
        Block getBlock(int x, int y, int z);

        void render(Renderer& renderer);
        bool createMesh();

      private:
        sf::Vector2i m_position;
        std::vector<ChunkSection> m_sections;
        std::vector<ChunkMesh> m_chunkMeshes;
    };
} // namespace client
