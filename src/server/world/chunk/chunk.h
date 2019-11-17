#pragma once

#include <SFML/Network/Packet.hpp>
#include <array>
#include <common/constants.h>

#include <common/world/chunk.h>

namespace server {
    class ServerChunk : public ChunkTEMP {
      public:
        ServerChunk(const ChunkPosition &chunkPosition);
        friend sf::Packet &operator<<(sf::Packet &packet,
                                      const ServerChunk &chunk);
    };

} // namespace server
