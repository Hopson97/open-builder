#pragma once

#include "../world/entity.h"
#include "session.h"
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/System/Clock.hpp>
#include <common/network/commands.h>
#include <common/network/endpoint.h>
#include <common/network/network_node.h>
#include <common/network/packet.h>
#include <common/network/packet_buffer.h>

namespace server {
    class Server final {
      public:
        Server(int maxConnections, port_t port, EntityArray &entities);

        void recievePackets();
        void resendPackets();
        void updatePlayers();

        int connectedPlayes() const;
        int maxConnections() const;
        int findEmptySlot() const;

        bool sendToClient(peer_id_t id, Packet &packet);
        void sendToAllClients(Packet &packet);

        Packet createPacket(CommandToClient command,
                            Packet::Flag flag = Packet::Flag::None);

      private:
        bool send(Packet &packet, const Endpoint &endpoint);

        bool recieve(Packet &packet, Endpoint &endpoint);

        void handleIncomingConnection(const Endpoint &endpoint);
        void handleDisconnect(sf::Packet &packet);
        void handleKeyInput(sf::Packet &packet);
        void handleAckPacket(sf::Packet &packet);

        std::vector<ClientSession> m_clientSessions;
        std::vector<ClientStatus> m_clientStatuses;
        std::vector<Endpoint> m_endpoints;

        int m_maxConnections = 4;
        int m_connections = 0;
        int m_aliveEntities = 0;

        sf::UdpSocket m_socket;

        PacketBuffer m_packetBuffer;

        peer_id_t m_nodeId;

        u32 m_sequenceNumber = 0;
    };
} // namespace server