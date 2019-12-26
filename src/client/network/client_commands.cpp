#include "../client.h"

#include <common/debug.h>
#include <common/world/chunk.h>
#include <thread>

void Client::sendDisconnectRequest()
{
    sf::Packet packet;
    packet << ServerCommand::Disconnect << NetworkHost::getPeerId();
    if (!sendToPeer(mp_serverPeer, packet, 0, ENET_PACKET_FLAG_RELIABLE)) {
        LOG("Client", "Failed to send disconnect packet");
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(64));

    NetworkHost::disconnectFromPeer(mp_serverPeer);
}

void Client::sendPlayerPosition(const glm::vec3 &position)
{
    sf::Packet packet;
    packet << ServerCommand::PlayerPosition << NetworkHost::getPeerId()
           << position.x << position.y << position.z;
    NetworkHost::sendToPeer(mp_serverPeer, packet, 0, 0);
}

void Client::sendChunkRequest(const ChunkPosition &position)
{
    sf::Packet packet;
    packet << ServerCommand::ChunkRequest << NetworkHost::getPeerId()
           << position.x << position.y << position.z;
    NetworkHost::sendToPeer(mp_serverPeer, packet, 0,
                            ENET_PACKET_FLAG_RELIABLE);
}

void Client::onPeerConnect([[maybe_unused]] ENetPeer *peer)
{
}

void Client::onPeerDisconnect([[maybe_unused]] ENetPeer *peer)
{
    m_status = EngineStatus::ExitServerDisconnect;
}

void Client::onPeerTimeout([[maybe_unused]] ENetPeer *peer)
{
    m_status = EngineStatus::ExitServerTimeout;
}

void Client::onCommandRecieve([[maybe_unused]] ENetPeer *peer,
                              sf::Packet &packet, command_t command)
{
    switch (static_cast<ClientCommand>(command)) {
        case ClientCommand::PlayerJoin:
            onPlayerJoin(packet);
            break;

        case ClientCommand::PlayerLeave:
            onPlayerLeave(packet);
            break;

        case ClientCommand::Snapshot:
            onSnapshot(packet);
            break;

        case ClientCommand::ChunkData:
            onChunkData(packet);
            break;

        case ClientCommand::PeerId:
            break;
    }
}

void Client::onPlayerJoin(sf::Packet &packet)
{
    peer_id_t id;
    packet >> id;
    m_entities[id].active = true;

    LOGVAR("Client", "Player joined, client id: ", (int)id);
}

void Client::onPlayerLeave(sf::Packet &packet)
{
    peer_id_t id;
    packet >> id;
    m_entities[id].active = false;

    LOGVAR("Client", "Player left, client id: ", (int)id);
}

void Client::onSnapshot(sf::Packet &packet)
{
    u16 updateEntityCount = 0;
    packet >> updateEntityCount;
    for (u16 i = 0; i < updateEntityCount; i++) {
        peer_id_t id = 0;
        float x, y, z;
        packet >> id >> x >> y >> z;
        if (id != NetworkHost::getPeerId()) {
            auto *p = &m_entities[id];
            p->position = {x, y, z};
            p->active = true;
        }
    }
}

void Client::onChunkData(sf::Packet &packet)
{
    ChunkPosition position;
    Chunk::Blocks blocks;

    packet >> position.x >> position.y >> position.z;
    for (auto &block : blocks) {
        packet >> block;
    }

    Chunk &chunk = m_chunks.manager.addChunk(position);
    chunk.blocks = std::move(blocks);
    m_chunks.updates.insert(position);
}
