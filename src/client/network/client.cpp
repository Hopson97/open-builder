#include "client.h"

#include "../game/client_world.h"
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
    using Cmd = ClientCommand;
    // clang-format off
    switch (packet.command()) {
        case Cmd::HandshakeChallenge:   onHandshakeChallenge    (packet);   break;
        case Cmd::ConnectionAcceptance: onConnectionAcceptance  (packet);   break;
        case Cmd::ForceExitGame:        onForceExit             (packet);   break;
           
        case Cmd::AddEntity:    onAddEntity     (packet);   break;
        case Cmd::RemoveEntity: onRemoveEntity  (packet);  break;
    }
    // clang-format on
}

ConnectionState Client::getConnnectionState() const
{
    return m_connectionState;
}

void Client::sendPlayerState(const EntityState& state)
{
    ClientPacket packet(ServerCommand::PlayerState, m_salt);
    packet.write(state.position);
    packet.write(state.rotation);
    m_serverConnection.send(packet.get());
}

void Client::onHandshakeChallenge(ClientPacket& packet)
{
    u32 salt = packet.read<u32>();
    u32 newSalt = m_salt ^ salt;
    m_salt = newSalt;
    ClientPacket response(ServerCommand::HandshakeResponse, m_salt);
    m_serverConnection.send(response.get());
}

void Client::onConnectionAcceptance(ClientPacket& packet)
{
    u8 isAccepted = packet.read<u8>();
    if (isAccepted) {
        std::cout << "Connected!\n";
        m_connectionState = ConnectionState::Connected;

        u32 playerId = packet.read<u32>();
        mp_world->setPlayerId(playerId);

        u32 count = packet.read<u32>();
        for (u32 i = 0; i < count; i++) {
            u32 id = packet.read<u32>();
            auto position = packet.read<glm::vec3>();
            auto rotation = packet.read<glm::vec3>();
            mp_world->addEntity(id, position, rotation);
        }
    }
    else {
        std::string reason = packet.read<std::string>();
        std::cout << "Rejected!\n" << reason << std::endl;
        m_connectionState = ConnectionState::Disconnected;
    }
}

void Client::onAddEntity(ClientPacket& packet)
{
    std::cout << "Player joined!\n";
    u32 count = packet.read<u32>();
    for (u32 i = 0; i < count; i++) {
        u32 entityId = packet.read<u32>();
        glm::vec3 position = packet.read<glm::vec3>();
        glm::vec3 rotation = packet.read<glm::vec3>();
        mp_world->addEntity(entityId, position, rotation);
    }
}

void Client::onRemoveEntity(ClientPacket& packet)
{
    u32 entityId = packet.read<u32>();
    mp_world->removeEntity(entityId);
    std::cout << "Player left!\n";
}

void Client::onForceExit(ClientPacket& packet)
{
    m_connectionState = ConnectionState::Disconnected;
    auto reason = packet.read<std::string>();
    std::cout << "Forced to leave game: " << reason << "\n";
}
