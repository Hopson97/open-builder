#include "../client.h"

#include <common/debug.h>
#include <common/world/chunk.h>
#include <thread>

namespace {
Chunk::CompressedBlocks getCompressedChunkFromPacket(sf::Packet &packet)
{
    u32 size;
    Chunk::CompressedBlocks compressed;
    packet >> size;
    for (u32 i = 0; i < size; i++) {
        block_t type;
        u16 count;
        packet >> type >> count;
        compressed.emplace_back(type, count);
    }
    return compressed;
}
} // namespace

void Client::sendPlayerPosition(const glm::vec3 &position)
{
    sf::Packet packet;
    packet << ServerCommand::PlayerPosition << NetworkHost::getPeerId()
           << position.x << position.y << position.z;
    NetworkHost::sendToPeer(mp_serverPeer, packet, 0, 0);
}

void Client::sendBlockUpdate(const BlockUpdate &update)
{
    sf::Packet packet;
    packet << ServerCommand::BlockEdit << update.position.x << update.position.y
           << update.position.z << update.block;
    NetworkHost::sendToPeer(mp_serverPeer, packet, 0, 0);
}

void Client::sendPlayerSkin(const sf::Image& playerSkin)
{
    // Check the image is the right size
    if (playerSkin.getSize() != sf::Vector2u(32, 64)) {
        LOG("Client", "Player's skin has the wrong dimensions");
        return;
    }

    sf::Packet packet;
    packet << ServerCommand::PlayerSkin << NetworkHost::getPeerId();
    packet.append(playerSkin.getPixelsPtr(), 8192);
    NetworkHost::sendToPeer(mp_serverPeer, packet, 0, ENET_PACKET_FLAG_RELIABLE);
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

        case ClientCommand::SpawnPoint:
            onSpawnPoint(packet);
            break;

        case ClientCommand::BlockUpdate:
            onBlockUpdate(packet);
            break;

        case ClientCommand::NewPlayerSkin:
            onPlayerSkinReceive(packet);
            break;

        case ClientCommand::PeerId:
            break;
    }
}

void Client::onPlayerJoin(sf::Packet &packet)
{
    peer_id_t id = 0;
    packet >> id;
    m_entities[id].active = true;

    LOGVAR("Client", "Player joined, client id: ", (int)id);
}

void Client::onPlayerLeave(sf::Packet &packet)
{
    peer_id_t id = 0;
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
    // Get position of the recieved chunk, and create a chunk
    ChunkPosition position;
    packet >> position.x >> position.y >> position.z;

    if (!m_chunks.manager.hasChunk(position)) {

        Chunk &chunk = m_chunks.manager.addChunk(position);

        // Uncompress the block data
        auto compressedData = getCompressedChunkFromPacket(packet);
        chunk.decompress(compressedData);

        // Add to chunk updates
        m_chunks.updates.push_back(position);
    }
}

void Client::onSpawnPoint(sf::Packet &packet)
{
    assert(mp_player);
    if (mp_player) {
        packet >> mp_player->position.x >> mp_player->position.y >>
            mp_player->position.z;
    }
}

void Client::onBlockUpdate(sf::Packet &packet)
{
    u16 count = 0;
    packet >> count;
    for (u32 i = 0; i < count; i++) {
        BlockUpdate blockUpdate;
        packet >> blockUpdate.position.x >> blockUpdate.position.y >> blockUpdate.position.z >>
            blockUpdate.block;
        m_chunks.blockUpdates.push_back(blockUpdate);
    }
}

void Client::onPlayerSkinReceive(sf::Packet& packet)
{
    peer_id_t id = 0;
    packet >> id;

    LOGVAR("Client", "Received skin for peer", (int)id);

    sf::Uint8* skinPixels = (sf::Uint8*)packet.getData() + sizeof(command_t) + sizeof(peer_id_t);
    m_entities[id].playerSkin.create(32, 64, skinPixels);
}