#pragma once

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/System/Clock.hpp>

#include <atomic>
#include <queue>
#include <vector>

#include "endpoint.h"
#include "entity.h"
#include <common/net/commands.h>

namespace server {
    class Server final {
      public:
        Server(int maxConnections);

        void run(sf::Time timeout = sf::seconds(7.5));

        bool isRunning() const;

      private:
        struct PackagedCommand {
            sf::Packet packet;
            sf::IpAddress address;
            CommandToServer command;
            Port port;
        };

        int findEmptySlot();

        void recievePackets();
        void update(float dt);
        void sendPackets();

        bool sendToClient(ClientId id, sf::Packet &packet);
        void sendToAllClients(sf::Packet &packet);
        bool getFromClient(PackagedCommand &package);

        void handleIncomingConnection(const sf::IpAddress &address, Port port);

        void handleDisconnect(sf::Packet &packet);
        void handleKeyInput(sf::Packet &packet);

        sf::UdpSocket m_socket;
        sf::Clock m_clock;
        sf::Clock m_exitClock;

        std::vector<ClientSession> m_clientSessions;
        std::vector<ClientStatus> m_clientStatuses;

        std::atomic<bool> m_isRunning = false;

        EntityArray m_entities;

        // std::queue<ClientEndpoint> m_pendingConnections;

        int m_maxConnections = 4;
        int m_connections = 0;
        int m_aliveEntities = 0;
    };
} // namespace server