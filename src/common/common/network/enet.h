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

    NetHost(const ENetAddress& address, int maximumConnections) noexcept;
    ~NetHost() noexcept;

    NetHost(NetHost&& other) noexcept;
    NetHost& operator=(NetHost&& other) noexcept;

    bool pumpEvent(NetEvent& event);

    NON_COPYABLE(NetHost)
};

template <typename Command>
struct Packet {
    Packet(ENetPacket* packet);

    Command command;
    u32 salt = 0;
    sf::Packet payload;
};

template <typename T>
sf::Packet createPacket(T command, u32 salt);
ENetPacket* createPacket(const sf::Packet& packet, u32 flags);
ClientConnectionResult connectEnetClientTo(ENetHost* host, Connection& serverConnection,
                                           const char* ipAddress);
bool disconnectEnetClient(ENetHost* host, Connection& serverConnection);
void broadcastToPeers(ENetHost* host, sf::Packet& packet, u8 channel = 0, u32 flags = 0);
u32 createHandshakeRandom();

template <typename Command>
inline Packet<Command>::Packet(ENetPacket* packet)

{
    payload.append(packet->data, packet->dataLength);
    payload >> command >> salt;
}

template <typename T>
inline sf::Packet createPacket(T command, u32 salt)
{
    sf::Packet packet;
    packet << command << salt;
    return packet;
}
