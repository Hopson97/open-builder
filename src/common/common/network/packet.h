#pragma once

#include "net_types.h"
#include <SFML/Network/UdpSocket.hpp>

/**
        Packet containing the data and also command
*/
struct Packet {
    sf::Packet data;
    Endpoint endpoint;
    command_t command = 0;
};

template <typename Command> Packet makePacket(Command command)
{
    Packet packet;
    packet.command = static_cast<command_t>(command);
    packet.data << packet.command;
    return packet;
}

bool receivePacket(sf::UdpSocket &socket, Packet &packet);