#pragma once

#include "../types.h"
#include <SFML/Network/IpAddress.hpp>

using port_t = u16;
using peer_id_t = u8;
using entityid_t = u16;
using command_t = u8;
using input_t = u8;

struct Endpoint {
    sf::IpAddress address;
    port_t port = 0;
    peer_id_t id = 0;
};

inline bool operator==(const Endpoint &left, const Endpoint &right)
{
    return left.port == right.port &&
           left.address.toInteger() == right.address.toInteger();
}
