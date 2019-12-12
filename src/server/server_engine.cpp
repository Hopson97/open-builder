#include "server_engine.h"

#include "server_config.h"
#include <SFML/System/Clock.hpp>
#include <array>
#include <common/debug.h>
#include <common/network/net_command.h>
#include <common/network/packet.h>
#include <ctime>
#include <iostream>
#include <random>
#include <thread>

#include <common/network/enet.h>

namespace {
struct Entity {
    float x = 0, y = 0, z = 0;
    bool active = false;
};

class ServerEngine {
  private:
    std::array<Entity, 512> m_entities;
    std::array<bool, MAX_CONNECTIONS> m_peerConnected;
    ENetAddress m_address;
    ENetHost *m_server = nullptr;

    bool m_isRunning = true;

  public:
    void run(const ServerConfig &config, sf::Time timeout)
    {
        // Create the enet server
        ENetAddress address{};
        address.host = ENET_HOST_ANY;
        address.port = DEFAULT_PORT;
        m_server = enet_host_create(&address, MAX_CONNECTIONS, 2, 0, 0);
        if (!m_server) {
            LOG("Server", "Failed to create server, exiting\n");
            return;
        }
        LOG("Server", "Server has been created.");

        // Start the main server loop
        sf::Clock timeoutClock;
        while (m_isRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));

            receivePackets();
            sendPackets();

            if (m_server->connectedPeers == 0) {
                if (timeoutClock.getElapsedTime() >= timeout) {
                    m_isRunning = false;
                }
            }
            else {
                timeoutClock.restart();
            }
        }

        enet_host_destroy(m_server);
    }

  private:
    void sendPackets()
    {
        sf::Packet packet;
        u16 count = m_server->connectedPeers;
        packet << ClientCommand::Snapshot << count;
        for (int i = 0; i < MAX_CONNECTIONS; i++) {
            if (m_peerConnected[i]) {
                packet << static_cast<peer_id_t>(i) << m_entities[i].x
                       << m_entities[i].y << m_entities[i].z;
            }
        }
        broadcastToPeers(packet, 0);
    }

    void receivePackets()
    {
        ENetEvent event;
        while (enet_host_service(m_server, &event, 0) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    onConnect(*event.peer);
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    onDataReceive(*event.packet);
                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    onDisconnect(*event.peer);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
                    onDisconnectTimeout(*event.peer);
                    break;

                case ENET_EVENT_TYPE_NONE:
                    break;
            }
        }
    }

    int emptySlot()
    {
        for (int i = 0; i < MAX_CONNECTIONS; i++) {
            if (!m_peerConnected[i]) {
                return i;
            }
        }
        return -1;
    }

    void onConnect(ENetPeer &peer)
    {
        LOG("Server", "Got connection event.");
        LOGVAR("Server", "New Connection Port: ", peer.address.port);

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
            broadcastToPeers(announcement, ENET_PACKET_FLAG_RELIABLE |
                                               ENET_PACKET_FLAG_NO_ALLOCATE);

            enet_host_flush(m_server);
        }
    }

    void broadcastToPeers(sf::Packet &packet, u32 flags)
    {
        ENetPacket *broadcast =
            enet_packet_create(packet.getData(), packet.getDataSize(), flags);
        enet_host_broadcast(m_server, 0, broadcast);
        enet_host_flush(m_server);
    }

    void onDisconnect(const ENetPeer &peer)
    {
        LOG("Server", "Got disconnect event.");
    }

    void onDisconnectTimeout(const ENetPeer &peer)
    {
        LOG("Server", "Got disconnect due to timeout event.");
    }

    void onDataReceive(const ENetPacket &packet)
    {
        sf::Packet pkt;
        pkt.append(packet.data, packet.dataLength);

        ServerCommand command;
        pkt >> command;
        switch (command) {
            case ServerCommand::PlayerPosition:
                handleCommandPlayerPosition(pkt);
                break;

            case ServerCommand::Disconnect:
                handleCommandDisconnect(pkt);
                break;
        }
    }

    void handleCommandDisconnect(sf::Packet &packet)
    {
        LOG("Server", "Disconnect command received");
        // Set connect flag to false for this client
        peer_id_t id;
        packet >> id;
        m_peerConnected[id] = false;

        // Broadcast the disconnection event
        sf::Packet announcement;
        announcement << ClientCommand::PlayerLeave << id;
        broadcastToPeers(announcement, ENET_PACKET_FLAG_RELIABLE);
    }

    void handleCommandPlayerPosition(sf::Packet &packet)
    {
        peer_id_t id;
        packet >> id;
        packet >> m_entities[id].x >> m_entities[id].y >> m_entities[id].z;
    }
};
} // namespace

void runServerEngine(const ServerConfig &config, sf::Time timeout)
{
    ServerEngine engine;
    engine.run(config, timeout);
    LOG("Server", "Server exiting");
}
