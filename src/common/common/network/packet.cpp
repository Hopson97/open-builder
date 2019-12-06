#include "packet.h"

bool receivePacket(sf::UdpSocket &socket, Packet &packet)
{
    sf::Packet data;
    if (socket.receive(data, packet.endpoint.address, packet.endpoint.port) ==
        sf::Socket::Done) {
		data >> packet.command;
        return true;
	}
    return false;
}
