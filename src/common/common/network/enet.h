#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <enet/enet.h>

#include "../types.h"
#include <SFML/Network/Packet.hpp>

ENetPacket* createPacket(const sf::Packet& packet, u32 flags);

struct Connection {
    ENetPeer* peer = nullptr;
    void send(const sf::Packet& packet, int channel = 0, u32 flags = 0);
};

struct ClientConnectionResult {
    ClientConnectionResult() = default;
    ClientConnectionResult(const char* message);

    const char* message = nullptr;
    bool success = true;

    const static ClientConnectionResult SUCCESS;
};

ClientConnectionResult connectEnetClientTo(ENetHost* host, Connection& serverConnection,
                                           const char* ipAddress);
bool disconnectEnetClient(ENetHost* host, Connection& serverConnection);