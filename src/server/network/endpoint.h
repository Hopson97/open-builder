#pragma once

#include <SFML/Network/IpAddress.hpp>
#include <common/network/constants.h>
#include <common/types.h>

namespace server {
    struct ClientSession {
        sf::IpAddress address;
        Port port = 0;
        Input keyState = 0;
    };

    enum class ClientStatus : u8 {
        Connected,
        Connecting,
        Disconnected,
    };
} // namespace server
