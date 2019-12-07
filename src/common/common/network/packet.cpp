#include "packet.h"

#include <iostream>

bool receivePacket(sf::UdpSocket &socket, Packet &packet)
{
    if (socket.receive(packet.data, packet.endpoint.address,
                       packet.endpoint.port) == sf::Socket::Done) {
        packet.data >> packet.command;
        return true;
    }
    return false;
}
