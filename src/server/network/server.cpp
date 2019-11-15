#include "server.h"

#include <SFML/Network/Packet.hpp>

#include <common/network/commands.h>
#include <common/network/input_state.h>

#include "../world/chunk/chunk.h"

#include <ctime>
#include <iostream>
#include <random>
#include <thread>

namespace server {
    Server::Server(int maxConnections, port_t port, EntityArray &entities)
        : m_clientSessions(maxConnections)
        , m_clientStatuses(maxConnections)
        , m_networkNode(maxConnections, port)
    {
        std::fill(m_clientStatuses.begin(), m_clientStatuses.end(),
                  ClientStatus::Disconnected);

        m_networkNode.setBlocking(false);
        m_networkNode.setNodeId(0);

        for (int i = 0; i < m_maxConnections; i++) {
            m_clientSessions[i].p_entity = &entities[i];
        }
    }

    int Server::connectedPlayes() const
    {
        return m_connections;
    }

    int Server::maxConnections() const
    {
        return m_maxConnections;
    }

    int Server::findEmptySlot() const
    {
        for (int i = 0; i < m_maxConnections; i++) {
            if (m_clientStatuses[i] == ClientStatus::Disconnected) {
                return i;
            }
        }
        return -1;
    }

    void Server::recievePackets()
    {
        Packet packet;
        Endpoint endpoint;
        while (m_networkNode.recieve(packet, endpoint)) {
            switch (static_cast<CommandToServer>(packet.command)) {
                case CommandToServer::PlayerInput:
                    handleKeyInput(packet.payload);
                    break;

                case CommandToServer::Acknowledgment:
                    m_networkNode.handleAckPacket(packet.payload);
                    break;

                case CommandToServer::Connect:
                    handleIncomingConnection(endpoint);
                    break;

                case CommandToServer::Disconnect:
                    handleDisconnect(packet.payload);
                    break;
            }
        }
    }

    void Server::resendPackets()
    {
        m_networkNode.resendPackets();
    }

    void Server::updatePlayers()
    {
        for (int i = 0; i < m_maxConnections; i++) {
            if (m_clientStatuses[i] == ClientStatus::Connected) {
                auto &player = *m_clientSessions[i].p_entity;
                auto input = m_clientSessions[i].keyState;

                auto isPressed = [input](auto key) {
                    return (input & key) == key;
                };

                if (isPressed(PlayerInput::Forwards)) {
                    player.moveForwards();
                }
                else if (isPressed(PlayerInput::Back)) {
                    player.moveBackwards();
                }
                if (isPressed(PlayerInput::Left)) {
                    player.moveLeft();
                }
                else if (isPressed(PlayerInput::Right)) {
                    player.moveRight();
                }
            }
        }
    }

    bool Server::sendToClient(peer_id_t id, Packet &packet)
    {
        if (m_clientStatuses[id] == ClientStatus::Connected) {
            bool result = m_networkNode.send(packet, m_networkNode.connections[id]);
            return result;
        }
        return false;
    }

    void Server::sendToAllClients(Packet &packet)
    {
        for (int i = 0; i < m_maxConnections; i++) {
            sendToClient(i, packet);
        }
    }

    Packet Server::createPacket(CommandToClient command, Packet::Flag flag)
    {
        return m_networkNode.createPacket(command, flag);
    }
} // namespace server
