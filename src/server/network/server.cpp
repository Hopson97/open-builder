#include "server.h"

#include <SFML/Network/Packet.hpp>
#include <common/network/commands.h>
#include <common/network/input_state.h>
#include <ctime>
#include <iostream>
#include <random>
#include <thread>

namespace server {
    Server::Server(int maxConnections, port_t port, EntityArray &entities)
        : m_clientSessions(maxConnections)
        , m_clientStatuses(maxConnections)
        , m_endpoints(maxConnections)
    {
        std::fill(m_clientStatuses.begin(), m_clientStatuses.end(),
                  ClientStatus::Disconnected);

        m_socket.setBlocking(false);
        m_socket.bind(port);

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
        while (recieve(packet, endpoint)) {
            switch (static_cast<CommandToServer>(packet.command)) {
                case CommandToServer::PlayerInput:
                    handleKeyInput(packet.payload);
                    break;

                case CommandToServer::Acknowledgment:
                    handleAckPacket(packet.payload);
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
        if (!m_packetBuffer.isEmpty()) {
            auto &packet = m_packetBuffer.begin();
            if (!packet.clients.empty()) {
                auto itr = packet.clients.begin();
                sendToClient(*itr, packet.packet);
                packet.clients.erase(itr);
            }
        }
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
            auto &endpoint = m_endpoints[id];
            bool result = m_socket.send(packet.payload, endpoint.address,
                                        endpoint.port) == sf::Socket::Done;
            if (packet.hasFlag(Packet::Flag::Reliable)) {
                m_packetBuffer.append(std::move(packet), endpoint.id);
            }
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
        return createCommandPacket(
            command, flag,
            flag == Packet::Flag::Reliable ? m_sequenceNumber++ : 0);
    }

    bool Server::recieve(Packet &packet, Endpoint &endpoint)
    {
        sf::Packet rawPacket;
        if (m_socket.receive(rawPacket, endpoint.address, endpoint.port) ==
            sf::Socket::Done) {
            packet.initFromPacket(rawPacket);
            return true;
        }
        return false;
    }
} // namespace server
