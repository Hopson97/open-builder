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

            if (m_server->connectedPeers == 0 &&
                timeoutClock.getElapsedTime() >= timeout) {
                m_isRunning = false;
            }
            else {
                timeoutClock.restart();
            }
        }

        enet_host_destroy(m_server);
    }

  private:
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

    void onConnect(const ENetPeer &peer)
    {
        LOG("Server", "Got connection event.");
        LOGVAR("Server", "New Connection Port: ", peer.address.port);
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
        sf::Packet buffer;
        client_id_t senderId;
        buffer.append(packet.data, packet.dataLength);
        
        ServerCommand command;
        buffer >> command;
        switch (command)
        {
            case ServerCommand::PlayerPosition:
                break;

            case ServerCommand::Disconnect:
                break;
        }
    }
};
} // namespace

void runServerEngine(const ServerConfig &config, sf::Time timeout)
{
    ServerEngine engine;
    engine.run(config, timeout);
    LOG("Server", "Server exiting");
}
