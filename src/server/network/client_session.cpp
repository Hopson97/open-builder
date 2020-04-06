#include "client_session.h"
#include <common/network/net_command.h>

#include "../world/server_world.h"
#include <common/debug.h>

void ClientSession::init(ENetPeer* peer, u32 salt, u32 playerId)
{
    m_clientConnection.salt = salt;
    m_salt = salt;
    m_clientConnection.peer = peer;
    m_playerId = playerId;
    m_isActive = true;
}

void ClientSession::disconnect()
{
    if (m_isActive) {
        enet_peer_disconnect(m_clientConnection.peer, 0);
        m_isActive = false;
    }
}

void ClientSession::sendPacket(const ServerPacket& packet, u32 channel, u32 flags)
{
    m_clientConnection.send(packet.get(), channel, flags);
}

void ClientSession::tick(ServerWorld& world)
{
    for (auto& chunk : world.getChunks()) {
        if (m_sentChunks.find(chunk.second.getPosition()) == m_sentChunks.end()) {
            sendAddChunk(chunk.second);
            m_sentChunks.insert(chunk.second.getPosition());
        }
    }
}

bool ClientSession::verify(u32 salt) const
{
    return salt == m_clientConnection.salt;
}

bool ClientSession::isActive() const
{
    return m_isActive;
}

u32 ClientSession::getPlayerId() const
{
    return m_playerId;
}

void ClientSession::sendAddChunk(const Chunk& chunk)
{
    ServerPacket outgoing(ClientCommand::AddChunk, m_salt);
    auto compressed = compressVoxelData(chunk.voxels);

    outgoing.write(chunk.getPosition().x);
    outgoing.write(chunk.getPosition().y);
    outgoing.write(chunk.getPosition().z);

    outgoing.write(static_cast<u32>(compressed.size()));
    for (auto& voxel : compressed) {
        outgoing.write(voxel.first).write(voxel.second);
    }

    m_clientConnection.send(outgoing.get(), 1, ENET_PACKET_FLAG_RELIABLE);
}

void ClientSession::sendPlayerSpawnPoint(const glm::vec3& position)
{
    ServerPacket outgoing(ClientCommand::PlayerSpawnPoint, m_salt);
    outgoing.write(position);

    m_clientConnection.send(outgoing.get(), 0, ENET_PACKET_FLAG_RELIABLE);
}

void ClientSession::sendVoxelUpdate(const VoxelUpdate& update)
{
    auto chunkPosition = toChunkPosition(update.voxelPosition);
    if (m_sentChunks.find(chunkPosition) != m_sentChunks.end()) {
        ServerPacket outgoing(ClientCommand::VoxelUpdate, m_salt);
        outgoing.write(update.voxelPosition.x);
        outgoing.write(update.voxelPosition.y);
        outgoing.write(update.voxelPosition.z);

        outgoing.write(update.voxel);

        m_clientConnection.send(outgoing.get(), 0, ENET_PACKET_FLAG_RELIABLE);
    }
}
