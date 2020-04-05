#include "../server_engine.h"

void ServerEngine::broadcastEntityStates()
{
    ServerPacket packet(ClientCommand::UpdateEntityStates, m_salt);
    m_world.serialiseEntities(packet);

    broadcastPacket(packet);
}

void ServerEngine::broadcastPlayerJoin(u32 playerId)
{
    ServerPacket packet(ClientCommand::AddEntity, m_salt);
    packet.write((u32)1);
    packet.write(playerId);
    std::cout << "Player join " << playerId << std::endl;
    auto& player = m_world.findEntity(playerId);
    packet.write(player.position);
    packet.write(player.rotation);

    broadcastToPeers(m_host.handle, packet.get(), 0, ENET_PACKET_FLAG_RELIABLE);
}

void ServerEngine::broadcastPlayerLeave(u32 playerId)
{
    ServerPacket packet(ClientCommand::RemoveEntity, m_salt);
    packet.write(playerId);
    broadcastToPeers(m_host.handle, packet.get(), 0, ENET_PACKET_FLAG_RELIABLE);
}

void ServerEngine::broadcastServerShutdown()
{
    ServerPacket packet(ClientCommand::ForceExitGame, m_salt);
    packet.write(std::string("Host has exited the game."));
    broadcastToPeers(m_host.handle, packet.get(), 0, ENET_PACKET_FLAG_RELIABLE);
}