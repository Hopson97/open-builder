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

namespace {
struct Entity {
    float x = 0, y = 0, z = 0;
    bool active = false;
};

class ServerEngine {
  private:
    std::array<Entity, MAX_CONNECTIONS> m_entities;

  public:
    void run(const ServerConfig &config, sf::Time timeout)
    {
        // Start the main server loop
        sf::Clock deltaClock;
        sf::Clock timeoutClock;

        bool isRunning = true;
        while (isRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            auto deltaTime = deltaClock.restart();

            receivePackets();
            tick();
/*
            if (m_server.clients.connectedCount() == 0) {
                if (timeoutClock.getElapsedTime() >= timeout) {
                    isRunning = false;
                }
            }
            else {
                timeoutClock.restart();
            }
*/
        }

    }

  private:
    void tick()
    {
        auto packet = makePacket(ClientCommand::Snapshot);
        /*
        auto count = static_cast<u16>(m_server.clients.connectedCount());
        packet.data << count;

        for (int i = 0; i < MAX_CONNECTIONS; i++) {
            if (m_server.clients.clientIsConnected(i)) {
                packet.data << static_cast<client_id_t>(i);
                packet.data << m_entities[i].x << m_entities[i].y
                            << m_entities[i].z;
            }
        }
        m_server.broadcastPacket(packet);
        */
    }

    void receivePackets()
    {
        Packet incoming;
        /*
        while (receivePacket(m_server.socket, incoming)) {
            processPacket(incoming);
        }
        */
    }

    void processPacket(Packet &packet)
    {
        auto command = static_cast<ServerCommand>(packet.command);
        switch (command) {
            case ServerCommand::PlayerPosition:
                handlePlayerPosition(packet);
                break;

            case ServerCommand::Connect:
                handleConnectRequest(packet);
                break;

            case ServerCommand::ConnectionChallengeResponse:
                break;

            case ServerCommand::Disconnect:
                handleDisconnect(packet);
                break;

            default:
                break;
        }
    }

    void handleConnectRequest(Packet &packet)
    {
        /*
        int slot = m_server.tryConnectClient(packet);
        if (slot >= 0) {
            m_entities[slot].active = true;
        }
        */
    }

    void handleDisconnect(Packet &packet)
    {
        /*
        int slot = m_server.tryDisconnectClient(packet);
        if (slot >= 0) {
            m_entities[slot].active = false;
        }
        */
    }

    void handlePlayerPosition(Packet &packet)
    {
        /*
        client_id_t id = 0;
        packet.data >> id;
        if (m_server.clients.clientIsConnected(id)) {
            auto *player = &m_entities[id];
            packet.data >> player->x >> player->y >> player->z;
        }
        */
    }
};
} // namespace

void runServerEngine(const ServerConfig &config, sf::Time timeout)
{
    ServerEngine engine;
    engine.run(config, timeout);
    LOG("Server", "Server exiting");
}
