#include "server.h"

#include "../server_config.h"
#include <SFML/System/Clock.hpp>
#include <common/debug.h>
#include <thread>

Server::Server()
    : NetworkHost("Server")
{
}

void Server::start(const ServerConfig &config, sf::Time timeout)
{
    NetworkHost::createAsServer(config.maxConnections);

    // Start the main server loop
    sf::Clock timeoutClock;
    while (m_isRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        NetworkHost::tick();
        sendPackets();

        if (NetworkHost::getConnectedPeerCount() == 0) {
            if (timeoutClock.getElapsedTime() >= timeout) {
                m_isRunning = false;
            }
        }
        else {
            timeoutClock.restart();
        }
    }

    NetworkHost::destroy();
}

void Server::onPeerConnect(ENetPeer &peer)
{
    int slot = emptySlot();
    if (slot >= 0) {
        peer_id_t id = static_cast<peer_id_t>(slot);

        // Send client back their id
        sf::Packet packet;
        packet << ClientCommand::ClientId << id;
        NetworkHost::sendToPeer(peer, packet, 0, ENET_PACKET_FLAG_RELIABLE);

        // Broadcast the connection event
        sf::Packet announcement;
        announcement << ClientCommand::PlayerJoin << id;
        broadcastToPeers(announcement, 0,
                         ENET_PACKET_FLAG_RELIABLE |
                             ENET_PACKET_FLAG_NO_ALLOCATE);

        addPeer(peer.connectID, id);
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

void Server::sendPackets()
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

int Server::emptySlot() const
{
    for (int i = 0; i < NetworkHost::getMaxConnections(); i++) {
        if (!m_peerConnected[i]) {
            return i;
        }
    }
    return -1;
}

void Server::addPeer(u32 connectionId, peer_id_t id)
{
    LOGVAR("Server", "New Peer, Peer Id:", (int)id);
    m_peerIds[connectionId] = id;
    m_peerConnected[id] = true;
}

void Server::removePeer(u32 connectionId)
{
    auto itr = m_peerIds.find(connectionId);
    if (itr != m_peerIds.end()) {
        int id = m_peerIds[connectionId];
        LOGVAR("Server", "Peer disconnect, Peer Id:", id);
        m_peerConnected[id] = false;
        m_peerIds.erase(itr);
    }
}