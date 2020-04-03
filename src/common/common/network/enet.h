#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <enet/enet.h>

#include "../macros.h"
#include "../types.h"
#include <SFML/Network/Packet.hpp>
#include <iostream>

struct ClientConnectionResult {
    ClientConnectionResult() = default;
    ClientConnectionResult(const char* message);

    const char* message = nullptr;
    bool success = true;

    const static ClientConnectionResult SUCCESS;
};

struct Connection {
    ENetPeer* peer = nullptr;
    u32 salt = 0;

    void send(const sf::Packet& packet, int channel = 0, u32 flags = 0);
};

enum class NetEventType {
    None = ENET_EVENT_TYPE_NONE,
    Data = ENET_EVENT_TYPE_RECEIVE,
    Connection = ENET_EVENT_TYPE_CONNECT,
    Disconnect = ENET_EVENT_TYPE_DISCONNECT,
    Timeout = ENET_EVENT_TYPE_DISCONNECT_TIMEOUT,
};

struct NetEvent {
    ENetEvent data;
    NetEventType type;

    ENetPacket* packet = nullptr;
    ENetPeer* peer = nullptr;
};

struct NetHost {
    ENetHost* handle = nullptr;

    NetHost();
    NetHost(const ENetAddress& address, int maximumConnections) noexcept;
    ~NetHost() noexcept;

    NetHost(NetHost&& other) noexcept;
    NetHost& operator=(NetHost&& other) noexcept;

    bool pumpEvent(NetEvent& event);

    NON_COPYABLE(NetHost)
};

ClientConnectionResult connectEnetClientTo(ENetHost* host, Connection& serverConnection,
                                           const char* ipAddress);
bool disconnectEnetClient(ENetHost* host, Connection& serverConnection);
void broadcastToPeers(ENetHost* host, const sf::Packet& packet, u8 channel = 0,
                      u32 flags = 0);
u32 createHandshakeRandom();
