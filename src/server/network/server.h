#pragma once

#include <array>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <common/network/net_constants.h>

struct ClientManager {
    std::array<Endpoint, MAX_CONNECTIONS> endpoints;
    std::array<bool, MAX_CONNECTIONS> isConnected{false};
    int currentConnections = 0;

	int emptySlot();
};

struct Server {
    ClientManager clients;
    sf::UdpSocket socket;
};