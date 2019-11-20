#pragma once

#include "../block/block.h"
#include "chunk_mesh.h"
#include <SFML/Network/Packet.hpp>
#include <common/constants.h>
#include <common/coordinate.h>

namespace client {
    class Renderer;
    class World;

    class ChunkSection final {
      public:
        ChunkSection(int x, int y, int z, World &world);

        Block getBlock(const BlockPosition &blockPosition) const;

        const ChunkSectionPosition &getPosition() const;

        friend sf::Packet &operator>>(sf::Packet &packet, ChunkSection &chunk);

      private:
        std::array<Block, CHUNK_VOLUME> m_blocks;
        World *mp_world;
        ChunkSectionPosition m_position;
    };

    class Chunk final {
      public:
        Chunk(const ChunkPosition &chunkPosition, World &world);

        void addSection(ChunkSection section);
        Block getBlock(const BlockPosition &blockPosition);

        void render(Renderer &renderer);
        bool createMesh();

        int countSections() const;
        const ChunkPosition &getPosition() const;

      private:
        ChunkPosition m_position;
        std::vector<ChunkSection> m_sections;
        std::vector<ChunkMeshObjects> m_chunkMeshes;

        World &mp_world;
    };
} // namespace client
