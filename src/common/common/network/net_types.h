#pragma once

#include "../types.h"
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>

// Network types
using port_t = u16;
using client_id_t = u8;
using command_t = u16;

/**
        End point of a connection
*/
struct Endpoint {
    sf::IpAddress address;
    port_t port = 0;
};