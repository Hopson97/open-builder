#pragma once

#include <SFML/Network/Packet.hpp>
#include <array>
#include <common/constants.h>
#include <common/world/chunk.h>
#include <iostream>

namespace client {
    struct ChunkState {
        enum class Flag : u8 {
            None = 0,
            NeedsNewMesh = 1,
        };

        ChunkState(Flag initialFlag)
            : flags(static_cast<u8>(initialFlag))
        {
        }

        u8 flags;
    };
} // namespace client
