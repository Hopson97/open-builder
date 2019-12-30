#include "server.h"

#include "../server_config.h"
#include <SFML/System/Clock.hpp>
#include <algorithm>
#include <common/debug.h>
#include <thread>

#include "../world/terrain_generation.h"

Server::Server()
    : NetworkHost("Server")
{
    // Create spawn area
    for (int cy = 0; cy < TEMP_WORLD_SIZE; cy++) {
        for (int cz = 0; cz < TEMP_WORLD_SIZE; cz++) {
            for (int cx = 0; cx < TEMP_WORLD_SIZE; cx++) {
                Chunk &chunk = m_world.chunks.addChunk({cx, cy, cz});
                makeFlatTerrain(&chunk);
            }
        }
    }
}

void Server::sendChunk(peer_id_t peerId, const Chunk &chunk)
{
    if (!m_connectedClients[peerId].connected) {
        return;
    }
    // Create the chunk-data packet
    sf::Packet packet;
    packet << ClientCommand::ChunkData << chunk.getPosition().x
           << chunk.getPosition().y << chunk.getPosition().z;

#ifdef OB_CHUNK_PACKET_OLD_STYLE
    // "Old Style" - Send entire chunk
    packet.append(chunk.blocks.data(),
                  chunk.blocks.size() * sizeof(chunk.blocks[0]));
#else
    // "New Style" - Compress the chunk before sending
    auto compressedChunk = chunk.compress();
    packet << static_cast<u32>(compressedChunk.size());
    for (auto &block : compressedChunk) {
        packet << block.first << block.second;
    }
#endif

    // Send chunk data to client
    sendToPeer(m_connectedClients[peerId].peer, packet, 1,
               ENET_PACKET_FLAG_RELIABLE);
}

void Server::onPeerConnect(ENetPeer *peer)
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
        // TODO: Actually calculate a position rather than use hardcoded numbers
        sf::Packet spawn;
        m_entities[id].x = 200;
        m_entities[id].y = CHUNK_SIZE * TEMP_WORLD_SIZE - CHUNK_SIZE + 2;
        m_entities[id].z = 200;
        spawn << ClientCommand::SpawnPoint << m_entities[id].x
              << m_entities[id].y << m_entities[id].z;
        sendToPeer(peer, spawn, 0, ENET_PACKET_FLAG_RELIABLE);

        auto cp = worldToChunkPosition(
            {m_entities[id].x, m_entities[id].y, m_entities[id].z});

        // Send the chunks at the "spawn"
        int radius = 2;
        for (int cy = cp.y - radius; cy <= cp.y + radius; cy++) {
            for (int cz = cp.z - radius; cz <= cp.z + radius; cz++) {
                for (int cx = cp.x - radius; cx <= cp.x + radius; cx++) {
                    Chunk &chunk = m_world.chunks.addChunk({cx, cy, cz});
                    sendChunk(id, chunk);
                }
            }
        }

        // Send the inital world to the client
        for (int cy = TEMP_WORLD_SIZE - 1; cy >= 0; cy--) {
            for (int cz = 0; cz < TEMP_WORLD_SIZE; cz++) {
                for (int cx = 0; cx < TEMP_WORLD_SIZE; cx++) {

                    if (m_world.chunks.hasChunk({cx, cy, cz})) {
                        auto &chunk = m_world.chunks.getChunk({cx, cy, cz});
                        sendChunk(id, chunk);
                    }
                    else {
                        Chunk &chunk = m_world.chunks.addChunk({cx, cy, cz});
                        makeFlatTerrain(&chunk);
                        sendChunk(id, chunk);
                    }
                }
            }
        }
    }
}

void Server::onPeerDisconnect(ENetPeer *peer)
{
    removePeer(peer->connectID);
}

void Server::onPeerTimeout(ENetPeer *peer)
{
    removePeer(peer->connectID);
}

void Server::onCommandRecieve([[maybe_unused]] ENetPeer *peer,
                              sf::Packet &packet, command_t command)
{
    switch (static_cast<ServerCommand>(command)) {
        case ServerCommand::PlayerPosition:
            handleCommandPlayerPosition(packet);
            break;
    }
}

void Server::handleCommandPlayerPosition(sf::Packet &packet)
{
    peer_id_t id;
    packet >> id;
    packet >> m_entities[id].x >> m_entities[id].y >> m_entities[id].z;
}

void Server::sendPackets()
{
    // Player positions
    {
        sf::Packet packet;
        u16 count = NetworkHost::getConnectedPeerCount();
        packet << ClientCommand::Snapshot << count;
        for (int i = 0; i < NetworkHost::getMaxConnections(); i++) {
            if (m_connectedClients[i].connected) {
                packet << static_cast<peer_id_t>(i) << m_entities[i].x
                       << m_entities[i].y << m_entities[i].z;
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

void Server::addPeer(ENetPeer *peer, peer_id_t id)
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
        [this, &connectionId](auto &conn) {
            return conn.peer && conn.peer->connectID == connectionId;
        });

    assert(itr != m_connectedClients.end());
    if (itr != m_connectedClients.end()) {
        LOGVAR("Server", "Client disconnected, Peer Id:", (int)itr->entityId);
        m_entities[itr->entityId].active = false;
        itr->connected = false;
        itr->peer = nullptr;

        // Broadcast the disconnection event
        sf::Packet announcement;
        announcement << ClientCommand::PlayerLeave << itr->entityId;
        broadcastToPeers(announcement, 0, ENET_PACKET_FLAG_RELIABLE);

        itr->entityId = 0;
    }
}