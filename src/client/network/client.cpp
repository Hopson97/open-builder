#include "client.h"

#include <common/network/net_command.h>
#include <common/network/net_constants.h>
#include <common/network/packet.h>
#include <ctime>
#include <iostream>
#include <limits>
#include <random>

namespace {

// auto createSalt()
// {
//     std::mt19937 rng(std::time(nullptr));
//     std::uniform_int_distribution<u64> dist(
//         u64(0), std::numeric_limits<u64>::max() - 1);
//     return dist(rng);
// }
   
// auto createConnectionPacket()
// {
//     // auto salt = createSalt();
//     auto conPacket = makePacket(ServerCommand::Connect);
//     // conPacket.data << salt;
//     return conPacket;
// }

} // namespace

bool ClientConnection::connectTo(const sf::IpAddress &address)
{
    m_serverEndpoint = {address, DEFAULT_PORT};
    auto conPacket = makePacket(ServerCommand::Connect);
    if (sendToServer(conPacket)) {
        Packet response;
        std::cout << "Sent\n";
        Endpoint end;
        if (receivePacket(m_socket, response, end) == sf::Socket::Done) {
            if (static_cast<ClientCommand>(response.command) ==
                ClientCommand::AcceptConnection) {
                std::cout << "Accepted\n";
                response.data >> m_clientId;
            }
            else if (static_cast<ClientCommand>(response.command) ==
                     ClientCommand::RejectConnection) {
                std::cout << "Rejected\n";
                return false;
            }
        }
    }
}

void ClientConnection::disconnect()
{
    auto disconnectPacket = makePacket(ServerCommand::Disconnect);
    disconnectPacket.data << m_clientId;
    for (int i = 0; i < 10; i++) {
        sendToServer(disconnectPacket);
	}
}

bool ClientConnection::sendToServer(Packet &packet)
{
    return m_socket.send(packet.data, m_serverEndpoint.address,
                         m_serverEndpoint.port) == sf::Socket::Done;
}

bool ClientConnection::isConnected() const
{
    return false;
}
