#include "client.h"

#include <common/debug.h>
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
    if (sendPacketToServer(conPacket)) {
        Packet response;
        LOG("Client", "Connection request sent");
        if (receivePacket(socket, response)) {
            if (static_cast<ClientCommand>(response.command) ==
                ClientCommand::AcceptConnection) {
                response.data >> m_clientId;
                LOGVAR("Client",
                       "Connection accepted, client ID: ", (int)m_clientId);
                socket.setBlocking(false);
                return true;
            }
            else if (static_cast<ClientCommand>(response.command) ==
                     ClientCommand::RejectConnection) {
                LOG("Client", "Connection rejected\n");
                return false;
            }
        }
    }
    LOG("Client", "Failed to connect\n");
    return false;
}

void ClientConnection::disconnect()
{
    auto disconnectPacket = makePacket(ServerCommand::Disconnect);
    disconnectPacket.data << m_clientId;
    for (int i = 0; i < 10; i++) {
        sendPacketToServer(disconnectPacket);
    }
}

bool ClientConnection::sendPacketToServer(Packet &packet)
{
    return socket.send(packet.data, m_serverEndpoint.address,
                       m_serverEndpoint.port) == sf::Socket::Done;
}

bool ClientConnection::isConnected() const
{
    return true;
}

client_id_t ClientConnection::getClientId() const
{
    return m_clientId;
}
