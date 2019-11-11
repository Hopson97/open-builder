#pragma once

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/System/Clock.hpp>

#include <atomic>
#include <queue>
#include <vector>

#include "../game/entity.h"
#include "endpoint.h"
#include <common/network/commands.h>

namespace server {
    class Server final {
      public:
        Server(int maxConnections, Port port, EntityArray &entities);

        void recievePackets();
        void sendPackets();

        void updatePlayers();

        int connectedPlayes() const;
        int maxConnections() const;

        bool sendToClient(ClientId id, sf::Packet &packet);
        void sendToAllClients(sf::Packet &packet);

      private:
        struct PackagedCommand {
            sf::Packet packet;
            sf::IpAddress address;
            CommandToServer command;
            Port port;
        };

        int findEmptySlot();

        void update(float dt);

        bool getFromClient(PackagedCommand &package);

        void handleIncomingConnection(const sf::IpAddress &address, Port port);

        void handleDisconnect(sf::Packet &packet);
        void handleKeyInput(sf::Packet &packet);

        sf::UdpSocket m_socket;
        sf::Clock m_clock;
        sf::Clock m_exitClock;

        std::vector<ClientSession> m_clientSessions;
        std::vector<ClientStatus> m_clientStatuses;

        // EntityArray m_entities;

        // std::queue<ClientEndpoint> m_pendingConnections;

        int m_maxConnections = 4;
        int m_connections = 0;
        int m_aliveEntities = 0;
    };
} // namespace server