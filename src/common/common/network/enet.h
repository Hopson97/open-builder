#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <enet/enet.h>
#include <SFML/Network/Packet.hpp>

enum class PacketFlag {
    None = 0,
    Reliable = ENET_PACKET_FLAG_RELIABLE,
    Unsequenced = ENET_PACKET_FLAG_UNSEQUENCED,
};

struct Connection {
    ENetPeer* peer = nullptr;
    void send(const sf::Packet& packet);
};