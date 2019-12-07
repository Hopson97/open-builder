#pragma once

#include "net_types.h"
#include <SFML/Network/UdpSocket.hpp>

/**
 * @brief Packet that is sent and recieved over the network
 */
struct Packet {
    //Data to send/ data that was received
    sf::Packet data;

    // Endpoint the data was received from
    Endpoint endpoint;

    //The command flag of the packet, should be either a ServerCommand or ClientCommand
    command_t command = 0;
};

/**
 * @brief Creates a Packet ready for sending
 * 
 * @tparam Command Command type, ServerCommand or ClientCommand
 * @param command The command to be sent over the packet
 * @return Packet The newly created packet
 */
template <typename Command> Packet makePacket(Command command)
{
    Packet packet;
    packet.command = static_cast<command_t>(command);
    packet.data << packet.command;
    return packet;
}

/**
 * @brief Receives a packet and fills in necessary information, such as the command 
 * 
 * @param socket The socket to receive the data via
 * @param packet The packet to be recieved
 * @return true Data was received
 * @return false Nothing was received
 */
bool receivePacket(sf::UdpSocket &socket, Packet &packet);