#pragma once

#include "../types.h"
#include <SFML/Network/IpAddress.hpp>

// Network types
using port_t = u16;
using client_id_t = u8;

struct Endpoint {
    sf::IpAddress address;
    port_t port = 0;
};