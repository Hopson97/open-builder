#pragma once

#include "../types.h"
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/Socket.hpp>

// Port, used for uh ports
using port_t = u16;

// Client ID, the number used for client naming over the network
using client_id_t = u8;

// Command, the numerical representation of a command sent over a network
using command_t = u16;

/**
 * @brief The IP address and Port of an endpoint
 */
struct Endpoint {
    sf::IpAddress address;
    port_t port = 0;
};