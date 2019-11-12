#pragma once

#include <SFML/Network/IpAddress.hpp>
#include <common/constants.h>
#include <common/types.h>

namespace server {
    struct ClientSession {
        sf::IpAddress address;
        port_t port = 0;
        input_t keyState = 0;
        Entity *p_entity = nullptr;
    };

    enum class ClientStatus : u8 {
        Connected,
        Connecting,
        Disconnected,
    };
} // namespace server
