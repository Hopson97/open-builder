#include "server.h"

#include <iostream>

namespace server {
    void Server::handleIncomingConnection(const Endpoint &endpoint)
    {
        std::cout << "Connection request got\n";

        auto sendRejection = [this, &endpoint](ConnectionResult result) {
            auto rejectPacket =
                createCommandPacket(CommandToClient::ConnectRequestResult);
            rejectPacket.payload << result;
            send(rejectPacket, endpoint);
        };

        // This makes sure there are not any duplicated connections
        for (const auto &session : m_endpoints) {
            if (endpoint == session) {
                return;
            }
        }

        if (m_connections < m_maxConnections) {
            auto slot = findEmptySlot();
            if (slot < 0) {
                sendRejection(ConnectionResult::GameFull);
            }
            // Connection can be made
            auto responsePacket =
                createCommandPacket(CommandToClient::ConnectRequestResult);
            responsePacket.payload << ConnectionResult::Success
                                   << static_cast<peer_id_t>(slot)
                                   << static_cast<u8>(m_maxConnections);

            m_clientStatuses[slot] = ClientStatus::Connected;
            m_endpoints[slot] = endpoint;
            m_clientSessions[slot].p_entity->position.y = 10;
            m_clientSessions[slot].p_entity->isAlive = true;
            m_clientSessions[slot].p_entity->speed = 4.0f;

            m_aliveEntities++;
            send(responsePacket, endpoint);

            m_connections++;
            std::cout << "Client Connected slot: " << (int)slot << '\n';

            auto joinPack = createPacket(CommandToClient::PlayerJoin);
            joinPack.payload << static_cast<peer_id_t>(slot);
            sendToAllClients(joinPack);
        }
        else {
            sendRejection(ConnectionResult::GameFull);
        }
        std::cout << std::endl;
    }

    void Server::handleDisconnect(sf::Packet &packet)
    {
        peer_id_t client;
        packet >> client;
        m_clientStatuses[client] = ClientStatus::Disconnected;
        m_clientSessions[client].p_entity->isAlive = false;
        m_connections--;
        m_aliveEntities--;
        std::cout << "Client Disonnected slot: " << (int)client << '\n';
        std::cout << std::endl;

        auto joinPack = createPacket(CommandToClient::PlayerLeave);
        joinPack.payload << client;
        sendToAllClients(joinPack);
    }

    void Server::handleKeyInput(sf::Packet &packet)
    {
        peer_id_t client;

        packet >> client;
        packet >> m_clientSessions[client].keyState;
        packet >> m_clientSessions[client].p_entity->rotation.x;
        packet >> m_clientSessions[client].p_entity->rotation.y;
    }

    void Server::handleAckPacket(sf::Packet &packet)
    {
        u32 sequence;
        peer_id_t id;
        packet >> sequence >> id;
        m_packetBuffer.tryRemove(sequence, id);
    }
} // namespace server
