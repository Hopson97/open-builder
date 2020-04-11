#include "client.h"

#include "../game/client_world.h"
#include "client.h"
#include <SFML/System/Clock.hpp>
#include <cassert>
#include <common/debug.h>
#include <common/network/net_command.h>
#include <common/network/net_constants.h>
#include <common/world/entity_state.h>
#include <iostream>
#include "../game/player.h"

Client::Client(ClientWorld& world, Player& player)
    : m_salt(createHandshakeRandom())
    , mp_world(&world)
    , mp_player(&player)
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

void Client::handlePacket(ClientPacket& packet)
{
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

////////////////////////////////////////////////////////
//
//          Packet handling functions
//
void Client::onHandshakeChallenge(ClientPacket& packet)
{
    u32 salt = packet.read<u32>();
    u32 newSalt = m_salt ^ salt;
    m_salt = newSalt;
    ClientPacket response(ServerCommand::HandshakeResponse, m_salt);
    m_serverConnection.send(response.get());
    std::cout << "Challenging\n";
}

void Client::onConnectionAcceptance(ClientPacket& packet)
{
    u8 isAccepted = packet.read<u8>();
    if (isAccepted) {
        std::cout << "Connected!\n";
        m_connectionState = ConnectionState::Connected;

        // For certain unit tests, the world doesn't exist
        if (!mp_world) {
            return;
        }

        u32 playerId = packet.read<u32>();
        mp_player->playerId = playerId;

        sendSpawnRequest();
    }
    else {
        std::string reason = packet.read<std::string>();
        std::cout << "Rejected!\n" << reason << std::endl;
        m_connectionState = ConnectionState::Disconnected;
    }
}

void Client::onGameData(ClientPacket& packet)
{
    u16 voxels = packet.read<u16>();

    mp_world->setVoxelTextureCount(voxels);

    for (u16 i = 0; i < voxels; i++) {
        auto voxel = packet.read<VoxelData>();
        mp_world->addVoxelType(std::move(voxel));
    }
    mp_world->initialiseCommonVoxels();

    u32 count = packet.read<u32>();
    for (u32 i = 0; i < count; i++) {
        u32 id = packet.read<u32>();
        auto position = packet.read<glm::vec3>();
        auto rotation = packet.read<glm::vec3>();
        mp_world->addEntity(id, position, rotation);
    }
}

void Client::onAddEntity(ClientPacket& packet)
{
    u32 count = packet.read<u32>();
    for (u32 i = 0; i < count; i++) {
        u32 entityId = 0;
        glm::vec3 position;
        glm::vec3 rotation;

        packet.read(entityId);
        packet.read(position);
        packet.read(rotation);

        if (!mp_world) {
            return;
        }
        mp_world->addEntity(entityId, position, rotation);
    }
}

void Client::onAddChunk(ClientPacket& packet)
{
    ChunkPosition position;
    position.x = packet.read<i32>();
    position.y = packet.read<i32>();
    position.z = packet.read<i32>();

    if (!mp_world->hasChunk(position)) {
        CompressedVoxels voxels;
        u32 blockCount = packet.read<u32>();
        for (u32 i = 0; i < blockCount; i++) {
            voxel_t type = packet.read<voxel_t>();
            u16 count = packet.read<u16>();
            voxels.emplace_back(type, count);
        }
        // For certain unit tests, the world doesn't exist
        if (!mp_world) {
            return;
        }
        mp_world->createChunkFromCompressed(position, voxels);
    }
}

void Client::onRemoveEntity(ClientPacket& packet)
{
    u32 entityId = packet.read<u32>();
    mp_world->removeEntity(entityId);
}

void Client::onForceExit(ClientPacket& packet)
{
    m_connectionState = ConnectionState::Disconnected;
    auto reason = packet.read<std::string>();
}

void Client::onPlayerSpawnPoint(ClientPacket& packet)
{
    glm::vec3 position = packet.read<glm::vec3>();
    
    mp_player->m_state.position = position;
}

void Client::onUpdateEntityStates(ClientPacket& packet)
{
    u32 count = packet.read<u32>();
    for (u32 i = 0; i < count; i++) {
        auto entId = packet.read<u32>();
        auto entPosition = packet.read<glm::vec3>();
        auto entRotation = packet.read<glm::vec3>();
        if (entId == mp_player->playerId) {
            continue;
        }
        mp_world->updateEntity(entId, entPosition, entRotation);
    }
}

void Client::onVoxelUpdate(ClientPacket& packet)
{
    VoxelUpdate update;
    packet.read(update.voxelPosition.x);
    packet.read(update.voxelPosition.y);
    packet.read(update.voxelPosition.z);
    packet.read(update.voxel);

    mp_world->updateVoxel(update);
}
