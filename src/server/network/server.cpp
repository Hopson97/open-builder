#include "server.h"

#include <SFML/System/Clock.hpp>
#include <common/debug.h>
#include <thread>

Server::Server()
    : NetworkHost("Server")
{
    std::cout << "Starting server\n";
}

void Server::start(const ServerConfig &config, sf::Time timeout)
{
    std::cout << "create seerver " << std::endl;
    NetworkHost::createAsServer();
    std::cout << "create seerver\n";

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
    peer_id_t slot = emptySlot();
    if (slot >= 0) {
        sf::Packet packet;
        packet << ClientCommand::ClientId << slot;
        ENetPacket *p = enet_packet_create(
            packet.getData(), packet.getDataSize(),
            ENET_PACKET_FLAG_RELIABLE | ENET_PACKET_FLAG_NO_ALLOCATE);
        enet_peer_send(&peer, 0, p);

        m_peerConnected[slot] = true;

        LOGVAR("Server", "New Connection Client ID: ", (int)slot);

        // Broadcast the connection event
        sf::Packet announcement;
        announcement << ClientCommand::PlayerJoin << slot;
        broadcastToPeers(announcement, 0,
                         ENET_PACKET_FLAG_RELIABLE |
                             ENET_PACKET_FLAG_NO_ALLOCATE);
    }
}

void Server::onPeerDisconnect(ENetPeer &peer)
{
}

void Server::onPeerTimeout(ENetPeer &peer)
{
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
    LOG("Server", "Disconnect command received");
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
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (m_peerConnected[i]) {
            packet << static_cast<peer_id_t>(i) << m_entities[i].x
                   << m_entities[i].y << m_entities[i].z;
        }
    }
    broadcastToPeers(packet, 0, 0);
}

int Server::emptySlot() const
{
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (!m_peerConnected[i]) {
            return i;
        }
    }
    return -1;
}