#include "server_engine.h"

#include "server_config.h"
#include <SFML/System/Clock.hpp>
#include <array>
#include <common/debug.h>
#include <common/network/net_command.h>
#include <common/network/net_host.h>
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

class ServerEngine : public NetworkHost {
  private:
    std::array<Entity, 512> m_entities;
    std::array<bool, MAX_CONNECTIONS> m_peerConnected;

    bool m_isRunning = true;

  public:
    ServerEngine()
        : NetworkHost("Server")
    {
    }

    void run(const ServerConfig &config, sf::Time timeout)
    {
        NetworkHost::createAsServer();

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

  private:
    void sendPackets()
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

    int emptySlot()
    {
        for (int i = 0; i < MAX_CONNECTIONS; i++) {
            if (!m_peerConnected[i]) {
                return i;
            }
        }
        return -1;
    }

    void onPeerConnect(ENetPeer &peer) override
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

    void onPeerDisconnect(ENetPeer &peer) override
    {
    }

    void onPeerTimeout(ENetPeer &peer) override
    {
    }

    void onCommandRecieve(sf::Packet &packet, command_t command) override
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
        broadcastToPeers(announcement, 0, ENET_PACKET_FLAG_RELIABLE);
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
