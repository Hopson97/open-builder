#pragma once

#include <SFML/Network/Packet.hpp>
#include <array>
#include <common/constants.h>
#include <iostream>

#include <common/world/chunk.h>

namespace client {
    class ClientChunk : public Chunk {
      public:
        enum class Flag : u8 {
            None = 0,
            NeedsNewMesh = 1,
        };

        ClientChunk(const ChunkPosition &chunkPosition);

        friend sf::Packet &operator>>(sf::Packet &packet, ClientChunk &chunk);


      public:
        Flag flag = Flag::None;
    };
} // namespace client
