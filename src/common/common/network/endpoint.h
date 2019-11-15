#pragma once

#include <SFML/Network/IpAddress.hpp>
#include "../types.h"

struct Endpoint
{
    sf::IpAddress address;
    port_t port;
    peer_id_t id;
};

inline bool operator==(const Endpoint& left, const Endpoint& right) 
{
    return left.port == right.port && left.address.toInteger() == right.address.toInteger();
}

struct HashEndpoint 
{
    size_t operator()(const Endpoint& endpoint) 
    {
        return (endpoint.address.toInteger() << 16) | endpoint.port;
    }
};