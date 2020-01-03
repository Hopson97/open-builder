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
    int size = TEMP_WORLD_SIZE;

    // Create spawn area
    for (int cz = 0; cz < TEMP_WORLD_SIZE; cz++) {
        for (int cx = 0; cx < TEMP_WORLD_SIZE; cx++) {
            std::array<int, CHUNK_AREA> heightMap = makeHeightMap({cx, 0, cz});

            for (int cy = 0; cy < 5; cy++) {
                Chunk &chunk = m_world.chunks.addChunk({cx, cy, cz});
                // makeFlatTerrain(&chunk);
                makeNaturalTerrain(&chunk, heightMap);
            }
        }
    }
    std::cout << "done\n";
}

void Server::sendChunk(peer_id_t peerId, const ChunkPosition &position)
{
    if (!m_connectedClients[peerId].connected) {
        return;
    }

    const Chunk &chunk = [this, &position]() {
        if (m_world.chunks.hasChunk(position)) {
            return m_world.chunks.getChunk(position);
        }
        else {
            Chunk &c = m_world.chunks.addChunk(position);
            return c;
        }
    }();

    // Create the chunk-data packet
    sf::Packet packet;
    packet << ClientCommand::ChunkData << chunk.getPosition().x
           << chunk.getPosition().y << chunk.getPosition().z;

#ifdef OLD_STYLE_NETWORK
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
        sf::Packet spawn;
        auto &player = m_entities[id];
        player.position = findPlayerSpawnPosition();
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
                    sendChunk(id, {cx, cy, cz});
                }
            }
        }

        // Send the inital world to the client
        for (int cy = 0; cy < TEMP_WORLD_SIZE; cy++) {
            for (int cz = 0; cz < TEMP_WORLD_SIZE; cz++) {
                for (int cx = 0; cx < TEMP_WORLD_SIZE; cx++) {
                    sendChunk(id, {cx, cy, cz});
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
    packet >> m_entities[id].position.x >> m_entities[id].position.y >>
        m_entities[id].position.z;
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

    assert(itr != m_connectedClients.cend());
    if (itr != m_connectedClients.cend()) {
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

glm::vec3 Server::findPlayerSpawnPosition()
{
    int x = (CHUNK_SIZE * TEMP_WORLD_SIZE) / 2;
    int z = (CHUNK_SIZE * TEMP_WORLD_SIZE) / 2;

    for (int chunkY = TEMP_WORLD_SIZE - 1; chunkY >= 0; chunkY--) {
        auto chunkPosition = worldToChunkPosition({x, 0, z});
        chunkPosition.y = chunkY;
        auto &spawn = m_world.chunks.getChunk(chunkPosition);

        for (int blockY = CHUNK_SIZE - 1; blockY >= 0; blockY--) {
            auto blockPosition = toLocalBlockPosition({x, 0, z});
            blockPosition.y = blockY;
            if (spawn.qGetBlock(blockPosition) > 0) {
                auto worldY = chunkY * CHUNK_SIZE + blockY + 3;
                return {x, worldY, z};
            }
        }
    }
    return {x, CHUNK_SIZE * TEMP_WORLD_SIZE, z};
}
