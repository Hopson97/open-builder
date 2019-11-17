#pragma once

#include <SFML/Network/Packet.hpp>
#include <array>
#include <common/constants.h>
#include <common/world/chunk.h>
#include <iostream>

namespace client {
    struct ChunkState {
        bool isReady = false;
        bool hasMesh = false;
        bool hasBuffer = false;
    };
} // namespace client
