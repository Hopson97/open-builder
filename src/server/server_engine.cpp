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
    std::array<ENetPeer *, MAX_CONNECTIONS> m_peers{nullptr};
    std::array<bool, MAX_CONNECTIONS> m_peerConnected;
    ENetHost *m_host;

  public:
    void run(const ServerConfig &config, sf::Time timeout)
    {

        // Start the main server loop
        sf::Clock deltaClock;
        sf::Clock timeoutClock;
        ENetAddress address{0};
        address.host = ENET_HOST_ANY;
        address.port = 12345;

        m_host = enet_host_create(&address, MAX_CONNECTIONS, 2, 0, 0);

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

        enet_host_destroy(m_host);
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
        ENetEvent event;
        if (enet_host_service(m_host, &event, 0)) {
            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE:
                    break;

                case ENET_EVENT_TYPE_CONNECT:
                    handleConnectRequest(event.peer);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
                case ENET_EVENT_TYPE_DISCONNECT:
                    // todo
                    break;

                default:
                    break;
            }
        }

        /*
        while (receivePacket(m_server.socket, incoming)) {
            processPacket(incoming);
        }
        */
    }

    void processPacket(ENetPacket *packet)
    {
        sf::Packet pkt;
        pkt.append(packet->data, packet->dataLength);
        ServerCommand command;
        pkt >> command;
        switch (command) {
            case ServerCommand::PlayerPosition:
                handlePlayerPosition(pkt);
                break;

            case ServerCommand::Disconnect:
                handleDisconnect(pkt);
                break;
        }
    }

    void handleConnectRequest(ENetPeer *peer)
    {
        for (int i = 0; i < MAX_CONNECTIONS; i++) {
            if (!m_peerConnected[i]) {
                m_peers[i] = peer;
                m_peerConnected[i] = true;

                // Tell client they are connected
                sf::Packet packet;
                packet << ClientCommand::AcceptConnection
                       << static_cast<client_id_t>(i);
                ENetPacket *enetpacket =
                    enet_packet_create(packet.getData(), packet.getDataSize(),
                                       ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(peer, 0, enetpacket);

                // Tell all clients a player has joined
                sf::Packet broadcast;
                broadcast << ClientCommand::PlayerJoin
                          << static_cast<client_id_t>(i);
                ENetPacket *enetbroadcast = enet_packet_create(
                    broadcast.getData(), broadcast.getDataSize(),
                    ENET_PACKET_FLAG_RELIABLE);

                for (int i = 0; i < MAX_CONNECTIONS; i++) {
                    if (m_peerConnected[i]) {
                        enet_peer_send(m_peers[i], 0, enetbroadcast);
                    }
                }

                enet_host_flush(m_host);
                enet_packet_destroy(enetpacket);
                enet_packet_destroy(enetbroadcast);
            }
        }
    }

    void handleDisconnect(sf::Packet &packet)
    {
        client_id_t id = 0;
        packet >> id;
        if (m_peers[id]) {
            m_peerConnected[id] = false;
            enet_peer_reset(m_peers[id]);
            // Tell all clients a player has joined
            sf::Packet broadcast;
            broadcast << ClientCommand::PlayerLeave
                      << static_cast<client_id_t>(id);
            ENetPacket *enetbroadcast =
                enet_packet_create(broadcast.getData(), broadcast.getDataSize(),
                                   ENET_PACKET_FLAG_RELIABLE);

            for (int i = 0; i < MAX_CONNECTIONS; i++) {
                if (m_peerConnected[i]) {
                    enet_peer_send(m_peers[i], 0, enetbroadcast);
                }
            }
            enet_host_flush(m_host);
            enet_packet_destroy(enetbroadcast);
        }
    }

    void handlePlayerPosition(sf::Packet &packet)
    {
        client_id_t id = 0;
        packet >> id;
        auto *player = &m_entities[id];
        packet >> player->x >> player->y >> player->z;
    }
};
} // namespace

void runServerEngine(const ServerConfig &config, sf::Time timeout)
{
    ServerEngine engine;
    engine.run(config, timeout);
    LOG("Server", "Server exiting");
}
