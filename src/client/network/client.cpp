#include "client.h"

#include <SFML/System/Clock.hpp>
#include <cassert>
#include <common/network/net_command.h>
#include <common/network/net_constants.h>
#include <iostream>

Client::Client()
    : m_salt(createHandshakeRandom())
{
}

Client::~Client()
{
    if (getConnnectionState() == ConnectionState::Connected) {
        disconnect();
    }
}

ClientConnectionResult Client::connectTo(const std::string& ipaddress)
{
    auto result =
        connectEnetClientTo(m_host.handle, m_serverConnection, ipaddress.c_str());
    if (result.success) {
        m_connectionState = ConnectionState::Pending;

        sf::Packet handshake;
        handshake << ServerCommand::HandshakePartOne << m_salt;
        m_serverConnection.send(handshake, 0, ENET_PACKET_FLAG_RELIABLE);
    }
    return result;
}

void Client::disconnect()
{
    if (m_connectionState != ConnectionState::Disconnected) {
        assert(m_host.handle);
        assert(m_serverConnection.peer);
        if (disconnectEnetClient(m_host.handle, m_serverConnection)) {
            m_connectionState = ConnectionState::Disconnected;
        }
    }
}

void Client::tick()
{
    assert(m_serverConnection.peer);
    assert(m_host.handle);
    NetEvent event;
    while (m_host.pumpEvent(event)) {
        if (event.type == NetEventType::Data) {
            ClientPacket packet(event.packet);
            handlePacket(packet);
            enet_packet_destroy(event.packet);
        }
    }
}

ConnectionState Client::getConnnectionState() const
{
    return m_connectionState;
}

void Client::handlePacket(ClientPacket& packet)
{
    using Cmd = ClientCommand;
    // clang-format off
    switch (packet.command()) {
        case Cmd::HandshakeChallenge:   onHandshakeChallenge(packet);   break;
        case Cmd::ConnectionAcceptance: onConnectionAcceptance(packet); break;
        case Cmd::ForceExitGame:        onForceExit(packet);            break;
           
        case Cmd::PlayerJoined:     onPlayerJoin(packet);   break;
        case Cmd::PlayerLeave:      onPlayerLeave(packet);  break;
    }
    // clang-format on
}

void Client::onHandshakeChallenge(ClientPacket& packet)
{
    u32 salt = packet.read<u32>();
    u32 newSalt = m_salt ^ salt;
    ClientPacket response(ServerCommand::HandshakeResponse, newSalt);
    m_serverConnection.send(response.get());
}

void Client::onConnectionAcceptance(ClientPacket& packet)
{
    u8 isAccepted = packet.read<u8>();
    if (isAccepted) {
        std::cout << "Connected!\n";
        m_connectionState = ConnectionState::Connected;
    }
    else {
        std::string reason = packet.read<std::string>();
        std::cout << "Rejected!\n" << reason << std::endl;
        m_connectionState = ConnectionState::Disconnected;
    }
}

void Client::onPlayerJoin(ClientPacket& packet)
{
    std::cout << "Player joined!\n";
}

void Client::onPlayerLeave(ClientPacket& packet)
{
    std::cout << "Player left!\n";
}

void Client::onForceExit(ClientPacket& packet)
{
    m_connectionState = ConnectionState::Disconnected;
    auto reason = packet.read<std::string>();
    std::cout << "Forced to leave game: " << reason << "\n";
}
