#include "server.h"

#include "../server_config.h"
#include <SFML/System/Clock.hpp>
#include <common/debug.h>
#include <thread>

#include "../world/terrain_generation.h"

Server::Server()
    : NetworkHost("Server")
{
}

ENetPeer *Server::findPeer(peer_id_t peerId)
{
    for (auto &[_, peer] : m_peerIds) {
        if (peer.peerId == peerId) {
            return peer.peer;
        }
    }
    return nullptr;
}

void Server::onPeerConnect(ENetPeer &peer)
{
    int slot = emptySlot();
    if (slot >= 0) {
        peer_id_t id = static_cast<peer_id_t>(slot);

        // Send client back their id
        sf::Packet packet;
        packet << ClientCommand::PeerId << id;
        NetworkHost::sendToPeer(peer, packet, 0, ENET_PACKET_FLAG_RELIABLE);

        // Broadcast the connection event
        sf::Packet announcement;
        announcement << ClientCommand::PlayerJoin << id;
        broadcastToPeers(announcement, 0,
                         ENET_PACKET_FLAG_RELIABLE |
                             ENET_PACKET_FLAG_NO_ALLOCATE);

        addPeer(&peer, id);
    }
}

void Server::onPeerDisconnect(ENetPeer &peer)
{
    removePeer(peer.connectID);
}

void Server::onPeerTimeout(ENetPeer &peer)
{
    removePeer(peer.connectID);
}

void Server::onCommandRecieve(sf::Packet &packet, command_t command)
{
    switch (static_cast<ServerCommand>(command)) {
        case ServerCommand::PlayerPosition:
            handleCommandPlayerPosition(packet);
            break;

        case ServerCommand::Disconnect:
            handleCommandDisconnect(packet);
            break;

        case ServerCommand::ChunkRequest:
            handleCommandChunkRequest(packet);
            break;
    }
}

void Server::handleCommandDisconnect(sf::Packet &packet)
{
    // Set connect flag to false for this client
    peer_id_t id;
    packet >> id;
    m_peerConnected[id] = false;

    // Broadcast the disconnection event
    sf::Packet announcement;
    announcement << ClientCommand::PlayerLeave << id;
    broadcastToPeers(announcement, 0, ENET_PACKET_FLAG_RELIABLE);
}

void Server::handleCommandPlayerPosition(sf::Packet &packet)
{
    peer_id_t id;
    packet >> id;
    packet >> m_entities[id].x >> m_entities[id].y >> m_entities[id].z;
}

void Server::handleCommandChunkRequest(sf::Packet &packet)
{
    peer_id_t id;
    ChunkPosition position;
    packet >> id >> position.x >> position.y >> position.z;

    m_chunkRequests.emplace(position, id);
}

void Server::sendPackets()
{
    // Player positions
    {
        sf::Packet packet;
        u16 count = NetworkHost::getConnectedPeerCount();
        packet << ClientCommand::Snapshot << count;
        for (int i = 0; i < NetworkHost::getMaxConnections(); i++) {
            if (m_peerConnected[i]) {
                packet << static_cast<peer_id_t>(i) << m_entities[i].x
                       << m_entities[i].y << m_entities[i].z;
            }
        }
        broadcastToPeers(packet, 0, 0);
    }

    // Chunks
    {
        // Add 1 per tick to the queue for now
        if (!m_chunkRequests.empty()) {

            auto cr = m_chunkRequests.front();
            m_chunkRequests.pop();

            Chunk &chunk = m_chunkManager.addChunk(cr.position);
            makeFlatTerrain(&chunk);


            // Create the chunk-data packet
            sf::Packet packet;
            packet << ClientCommand::ChunkData << cr.position.x << cr.position.y
                   << cr.position.z;
            for (auto &block : chunk.blocks) {
                packet << block;
            }
            packet.append(chunk.blocks.data(),
                          chunk.blocks.size() * sizeof(chunk.blocks[0]));

            // Send chunk data to client
            auto peer = findPeer(cr.peer);
            sendToPeer(*peer, packet, 1, ENET_PACKET_FLAG_RELIABLE);
        }
    }
}

int Server::emptySlot() const
{
    for (int i = 0; i < NetworkHost::getMaxConnections(); i++) {
        if (!m_peerConnected[i]) {
            return i;
        }
    }
    return -1;
}

void Server::addPeer(ENetPeer *peer, peer_id_t id)
{
    LOGVAR("Server", "New Peer, Peer Id:", (int)id);
    m_peerIds[peer->connectID] = {peer, id};
    m_peerConnected[id] = true;
}

void Server::removePeer(u32 connectionId)
{
    auto itr = m_peerIds.find(connectionId);
    if (itr != m_peerIds.end()) {
        int id = m_peerIds[connectionId].peerId;
        LOGVAR("Server", "Peer disconnect, Peer Id:", id);
        m_peerConnected[id] = false;
        m_peerIds.erase(itr);
    }
}