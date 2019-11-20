#pragma once

#include "../block/block.h"
#include <array>
#include <common/constants.h>
#include <common/network/packet.h>
#include <vector>

namespace server {
    class Server;
    class ChunkSection final {
      public:
        ChunkSection(const ChunkSectionPosition &chunkPosition);

        Block getBlock(const BlockPosition &BlockPosition) const;
        void setBlock(const BlockPosition &BlockPosition, Block block);
        void quickSetBlock(const BlockPosition &BlockPosition, Block block);

        const ChunkSectionPosition &getPosition() const;

        friend sf::Packet &operator<<(sf::Packet &packet,
                                      const ChunkSection &chunk);

      private:
        std::array<Block, CHUNK_VOLUME> m_blocks;
        ChunkSectionPosition m_position;
    };

    class Chunk final {
      public:
        Chunk(const ChunkPosition &position);

        void addSection(int chunkY);

        void setBlock(int x, int y, int z, Block block);
        Block getBlock(int x, int y, int z);

        void generateTerrain();
        bool readyForSend() const;

        void sendChunks(Server &server) const;

      private:
        ChunkPosition m_position;
        std::vector<ChunkSection> m_sections;

        bool m_isTerrainGenerated = false;
    };

} // namespace server
