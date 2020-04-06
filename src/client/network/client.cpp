#include "client.h"

#include <SFML/System/Clock.hpp>
#include <cassert>
#include <common/network/net_command.h>
#include <common/network/net_constants.h>
#include <common/world/entity_state.h>
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

void Client::setWorld(ClientWorld& world)
{
    mp_world = &world;
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

void Client::handlePacket(ClientPacket& packet)
{
    std::cout << "Client got packet: " << (int)packet.command() << std::endl;
    using Cmd = ClientCommand;
    // clang-format off
    switch (packet.command()) {
        case Cmd::HandshakeChallenge:   onHandshakeChallenge    (packet);   break;
        case Cmd::ConnectionAcceptance: onConnectionAcceptance  (packet);   break;
        case Cmd::ForceExitGame:        onForceExit             (packet);   break;
        case Cmd::GameData:             onGameData              (packet);   break;

        case Cmd::AddEntity:            onAddEntity             (packet);   break;
        case Cmd::AddChunk:             onAddChunk              (packet);   break;
        case Cmd::PlayerSpawnPoint:     onPlayerSpawnPoint      (packet);   break;
        case Cmd::RemoveEntity:         onRemoveEntity          (packet);   break;
        case Cmd::UpdateEntityStates:   onUpdateEntityStates    (packet);   break;
        case Cmd::VoxelUpdate:          onVoxelUpdate           (packet);   break;

        default: std::cout << "Unhandled packet! Command ID: " << (int)packet.command() << '\n'; break;
    }
    // clang-format on
}

ConnectionState Client::getConnnectionState() const
{
    return m_connectionState;
}

void Client::sendInteraction()
{
    ClientPacket packet(ServerCommand::Interaction, m_salt);
    m_serverConnection.send(packet.get());
}

void Client::sendMouseEvent(MouseEventState state)
{
    ClientPacket packet(ServerCommand::MouseState, m_salt);
    packet.write(static_cast<u8>(state == MouseEventState::Click ? 0 : 1));
    m_serverConnection.send(packet.get());
}

void Client::sendPlayerState(const EntityState& state)
{
    ClientPacket packet(ServerCommand::PlayerState, m_salt);
    packet.write(state.position);
    packet.write(state.rotation);
    m_serverConnection.send(packet.get());
}

void Client::sendSpawnRequest()
{
    ClientPacket packet(ServerCommand::SpawnRequest, m_salt);
    m_serverConnection.send(packet.get());
}
