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
            std::cout << "Got a event " << event.peer->incomingPeerID << std::endl;
            ClientPacket packet(event.packet);
            if (m_connectionState == ConnectionState::Connected) {
                handlePacket(packet);
            }
            else if (m_connectionState == ConnectionState::Pending)
                handlePendingPacket(packet);
        }
        enet_packet_destroy(event.packet);
    }
}

ConnectionState NetworkClient::getConnnectionState() const
{
    return m_connectionState;
}

void NetworkClient::handlePendingPacket(ClientPacket& packet)
{
    auto command = static_cast<ClientCommand>(packet.command);
    switch (command) {
    }
}

void NetworkClient::handlePacket(ClientPacket& packet)
{
}