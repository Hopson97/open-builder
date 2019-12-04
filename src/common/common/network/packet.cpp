#include "packet.h"

bool receivePacket(sf::UdpSocket &socket, Packet &packet, Endpoint &endpoint)
{
    sf::Packet data;
    if (socket.receive(data, endpoint.address, endpoint.port) ==
        sf::Socket::Done) {
		data >> packet.command;
        return true;
	}
    return false;
}
