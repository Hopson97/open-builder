#include "network_client.h"

#include <SFML/System/Clock.hpp>
#include <cassert>
#include <common/network/net_command.h>
#include <common/network/net_constants.h>
#include <iostream>

NetworkClient::NetworkClient()
    : mp_host(enet_host_create(nullptr, 1, 2, 0, 0))
    , m_salt(createHandshakeRandom())
{
}

NetworkClient::~NetworkClient()
{
    if (mp_host) {
        enet_host_destroy(mp_host);
    }

    if (getConnnectionState() == ConnectionState::Connected) {
        disconnect();
    }
}

ClientConnectionResult NetworkClient::connectTo(const std::string& ipaddress)
{
    auto result = connectEnetClientTo(mp_host, m_serverConnection, ipaddress.c_str());
    if (result.success) {
        m_connectionState = ConnectionState::Pending;

        sf::Packet handshake;
        handshake << ServerCommand::HandshakePartOne << m_salt;
        m_serverConnection.send(handshake, 0, ENET_PACKET_FLAG_RELIABLE);
    }
    return result;
}

void NetworkClient::disconnect()
{
    assert(mp_host);
    assert(m_serverConnection.peer);
    if (disconnectEnetClient(mp_host, m_serverConnection)) {
        m_connectionState = ConnectionState::Disconnected;
    }
}

void NetworkClient::tick()
{
    assert(m_serverConnection.peer);
    assert(mp_host);
    ENetEvent event;
    while (enet_host_service(mp_host, &event, 0) > 0) {
        if (event.type == ENET_EVENT_TYPE_RECEIVE) {
            ClientPacket packet(event.packet);
            handlePacket(packet);
            enet_packet_destroy(event.packet);
        }
       
    }
}

ConnectionState NetworkClient::getConnnectionState() const
{
    return m_connectionState;
}

void NetworkClient::handlePacket(ClientPacket& packet)
{
    using Cmd = ClientCommand;
    std::cout << "Got a event " << (int)packet.command << std::endl;
    // clang-format off
    switch (packet.command) {
        case Cmd::HandshakeChallenge: onHandshakeChallenge(packet); break;
        default: break;
    }
    // clang-format on

}

void NetworkClient::onHandshakeChallenge(ClientPacket& packet)
{
    u32 salt = 0;
    packet.payload >> salt;
    u32 newSalt = m_salt ^ salt;

    auto response = createPacket(ServerCommand::HandshakeResponse, newSalt);
    m_serverConnection.send(response);
}
