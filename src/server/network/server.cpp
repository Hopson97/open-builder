#include "server.h"

#include "../server_config.h"
#include <SFML/System/Clock.hpp>
#include <algorithm>
#include <common/debug.h>
#include <thread>

#include "../world/terrain_generation.h"

Server::Server(const ServerConfig& config)
    : NetworkHost("Server")
    , m_worldSize(config.worldSize)
    , m_worldHeight(config.worldHeight)
{
    for (int z = 0; z < m_worldSize; z++) {
        for (int x = 0; x < m_worldSize; x++) {
            std::array<int, CHUNK_AREA> heightMap = makeHeightMap({ x, 0, z });

            for (int y = 0; y < m_worldHeight; y++) {
                Chunk& chunk = m_world.chunks.addChunk({ x, y, z });
                //makeFlatTerrain(&chunk);
                makeNaturalTerrain(&chunk, heightMap, m_worldSize);
            }
        }
    }
}

void Server::sendChunk(peer_id_t peerId, const ChunkPosition& position)
{
    if (!m_connectedClients[peerId].connected) {
        return;
    }

    const Chunk& chunk = [this, &position]() {
        if (m_world.chunks.hasChunk(position)) {
            return m_world.chunks.getChunk(position);
        }
        else {
            Chunk& c = m_world.chunks.addChunk(position);
            return c;
        }
    }();

    // Create the chunk-data packet
    sf::Packet packet;
    packet << ClientCommand::ChunkData << chunk.getPosition().x
        << chunk.getPosition().y << chunk.getPosition().z;

    auto compressedChunk = chunk.compress();
    packet << static_cast<u32>(compressedChunk.size());
    for (auto& block : compressedChunk) {
        packet << block.first << block.second;
    }

    // Send chunk data to client
    sendToPeer(m_connectedClients[peerId].peer, packet, 1,
        ENET_PACKET_FLAG_RELIABLE);
}

void Server::sendPlayerSkin(peer_id_t peerId, peer_id_t toPeer)
{
    sf::Packet skinPacket;
    skinPacket << ClientCommand::NewPlayerSkin;

    for (int i = 0; i < (1024 * 8); i++)
        skinPacket << m_entities[peerId].m_skinData[i];

    if (toPeer == NULL)
        broadcastToPeers(skinPacket, 0, ENET_PACKET_FLAG_RELIABLE);
    else
        sendToPeer(m_connectedClients[toPeer].peer, skinPacket, 0, ENET_PACKET_FLAG_RELIABLE);
}

void Server::onPeerConnect(ENetPeer* peer)
{
    int slot = findEmptySlot();
    if (slot >= 0) {
        peer_id_t id = static_cast<peer_id_t>(slot);

        // Send client back their id
        sf::Packet packet;
        packet << ClientCommand::PeerId << id;
        NetworkHost::sendToPeer(peer, packet, 0, ENET_PACKET_FLAG_RELIABLE);

        // Broadcast the connection event
        sf::Packet announcement;
        announcement << ClientCommand::PlayerJoin << id;
        broadcastToPeers(announcement, 0, ENET_PACKET_FLAG_RELIABLE);

        addPeer(peer, id);

        // Send the spawn position
        sf::Packet spawn;
        auto& player = m_entities[id];
        player.position = findPlayerSpawnPosition();
        player.m_skinData.resize(8192);
        spawn << ClientCommand::SpawnPoint << player.position.x
            << player.position.y << player.position.z;
        sendToPeer(peer, spawn, 0, ENET_PACKET_FLAG_RELIABLE);

        // Send chunks around the player to the client (Spawn chunks)
        // "Radius" and "player chunk" position
        int r = 2;
        ChunkPosition pc = worldToChunkPosition(player.position);
        for (int cy = pc.y - r; cy <= pc.y + r; cy++) {
            for (int cz = pc.z - r; cz < pc.z + r; cz++) {
                for (int cx = pc.x - r; cx < pc.z + r; cx++) {
                    sendChunk(id, { cx, cy, cz });
                }
            }
        }

        // Send the inital world to the client
        for (int cy = 0; cy < m_worldSize; cy++) {
            for (int cz = 0; cz < m_worldSize; cz++) {
                for (int cx = 0; cx < m_worldSize; cx++) {
                    sendChunk(id, { cx, cy, cz });
                }
            }
        }

        // Send the peer other player's skins 
        // (Note: Could this overwhelm the player's buffer?)
        for (int p_id = 0; p_id < m_entities.size(); p_id++)
            if (m_entities[p_id].hasSkin)
                sendPlayerSkin(p_id, id);
    }
}

void Server::onPeerDisconnect(ENetPeer* peer)
{
    removePeer(peer->connectID);
}

void Server::onPeerTimeout(ENetPeer* peer)
{
    removePeer(peer->connectID);
}

void Server::onCommandRecieve([[maybe_unused]] ENetPeer* peer,
    sf::Packet& packet, command_t command)
{
    switch (static_cast<ServerCommand>(command)) {
    case ServerCommand::PlayerPosition:
        handleCommandPlayerPosition(packet);
        break;

    case ServerCommand::BlockEdit:
        handleCommandBlockEdit(packet);
        break;

    case ServerCommand::PlayerSkin:
        handleCommandPlayerSkin(packet);
        break;
    }
}

void Server::handleCommandPlayerPosition(sf::Packet& packet)
{
    peer_id_t id;
    packet >> id;
    packet >> m_entities[id].position.x >> m_entities[id].position.y >>
        m_entities[id].position.z;
}

void Server::handleCommandBlockEdit(sf::Packet& packet)
{
    BlockPosition position;
    block_t block;
    packet >> position.x >> position.y >> position.z >> block;
    m_blockUpdates.push_back({ position, block });
}

void Server::handleCommandPlayerSkin(sf::Packet& packet)
{
    LOGVAR("Server", "Received player skin, packet size: ", (int)packet.getDataSize());

    if (packet.getDataSize() != (sizeof(command_t) + sizeof(peer_id_t) + 8192))
    {
        LOG("Server", "Player Skin Packet is of an invalid size");
        return;
    }

    peer_id_t id;
    packet >> id;

    for (int i = 0; i < (1024 * 8); i++) {
        packet >> m_entities[id].m_skinData[i];
    }

    m_entities[id].hasSkin = true;

    sendPlayerSkin(id);
}

void Server::update()
{
    sf::Packet packet;
    if (m_blockUpdates.size() > 0) {

        u16 size = m_blockUpdates.size();
        packet << ClientCommand::BlockUpdate << size;

        for (auto& blockUpdate : m_blockUpdates) {
            auto chunkPosition = toChunkPosition(blockUpdate.position);
            m_world.chunks.ensureNeighbours(chunkPosition);
            m_world.chunks.setBlock(blockUpdate.position, blockUpdate.block);

            packet << blockUpdate.position.x << blockUpdate.position.y
                << blockUpdate.position.z << blockUpdate.block;
        }
        // TODO: Try find a way to not send block updates to players that
        // created them
        broadcastToPeers(packet, 0, ENET_PACKET_FLAG_RELIABLE);
        m_blockUpdates.clear();
    }

    // Player positions
    {
        sf::Packet packet;
        u16 count = NetworkHost::getConnectedPeerCount();
        packet << ClientCommand::Snapshot << count;
        for (int i = 0; i < NetworkHost::getMaxConnections(); i++) {
            if (m_connectedClients[i].connected) {
                packet << static_cast<peer_id_t>(i) << m_entities[i].position.x
                    << m_entities[i].position.y << m_entities[i].position.z;
            }
        }
        broadcastToPeers(packet, 0, 0);
    }
}

int Server::findEmptySlot() const
{
    for (int i = 0; i < NetworkHost::getMaxConnections(); i++) {
        if (!m_connectedClients[i].connected) {
            return i;
        }
    }
    return -1;
}

void Server::addPeer(ENetPeer* peer, peer_id_t id)
{
    LOGVAR("Server", "New Peer, Peer Id:", (int)id);
    m_connectedClients[id].peer = peer;
    m_connectedClients[id].connected = true;
    m_connectedClients[id].entityId = id;
}

void Server::removePeer(u32 connectionId)
{
    auto itr = std::find_if(
        m_connectedClients.begin(), m_connectedClients.end(),
        [this, &connectionId](auto& conn) {
            return conn.peer && conn.peer->connectID == connectionId;
        });

    assert(itr != m_connectedClients.cend());
    if (itr != m_connectedClients.cend()) {
        LOGVAR("Server", "Client disconnected, Peer Id:", (int)itr->entityId);
        m_entities[itr->entityId].active = false;
        m_entities[itr->entityId].hasSkin = false;
        itr->connected = false;
        itr->peer = nullptr;

        // Broadcast the disconnection event
        sf::Packet announcement;
        announcement << ClientCommand::PlayerLeave << itr->entityId;
        broadcastToPeers(announcement, 0, ENET_PACKET_FLAG_RELIABLE);

        itr->entityId = 0;
    }
}

glm::vec3 Server::findPlayerSpawnPosition()
{
    int x = (CHUNK_SIZE * m_worldSize) / 2;
    int z = (CHUNK_SIZE * m_worldSize) / 2;

    for (int chunkY = m_worldSize - 1; chunkY >= 0; chunkY--) {
        auto chunkPosition = worldToChunkPosition({ x, 0, z });
        chunkPosition.y = chunkY;
        auto& spawn = m_world.chunks.getChunk(chunkPosition);

        for (int blockY = CHUNK_SIZE - 1; blockY >= 0; blockY--) {
            auto blockPosition = toLocalBlockPosition({ x, 0, z });
            blockPosition.y = blockY;
            if (spawn.qGetBlock(blockPosition) > 0) {
                auto worldY = chunkY * CHUNK_SIZE + blockY + 3;
                return { x, worldY, z };
            }
        }
    }
    return { x, CHUNK_SIZE * m_worldSize, z };
}
