#include "client.h"

#include "../game/client_world.h"
#include <common/debug.h>

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

        // For certain unit tests, the world doesn't exist
        if (!mp_world) {
            return;
        }

        u32 playerId = packet.read<u32>();
        mp_world->setPlayerId(playerId);

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

        if (!mp_world)
        {
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
    mp_world->getPlayer().position = position;
}

void Client::onUpdateEntityStates(ClientPacket& packet)
{
    u32 count = packet.read<u32>();
    for (u32 i = 0; i < count; i++) {
        auto entId = packet.read<u32>();
        auto entPosition = packet.read<glm::vec3>();
        auto entRotation = packet.read<glm::vec3>();
        if (entId == mp_world->getPlayerId()) {
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
