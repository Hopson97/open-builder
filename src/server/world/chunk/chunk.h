#pragma once

#include "chunk_section.h"
#include <common/network/packet.h>
#include <vector>

namespace server {
    class Server;

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
    };

} // namespace server
