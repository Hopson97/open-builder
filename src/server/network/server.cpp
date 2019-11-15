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
        PackagedCommand package;
        while (getFromClient(package)) {
            auto &packet = package.packet;
            switch (static_cast<CommandToServer>(packet.command)) {
                case CommandToServer::PlayerInput:
                    handleKeyInput(packet.payload);
                    break;

                case CommandToServer::Acknowledgment:
                    handleAckPacket(packet.payload);
                    break;

                case CommandToServer::Connect:
                    handleIncomingConnection(package.endpoint);
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
                sendToClient(*packet.clients.begin(), packet.packet);
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
            bool result =
                m_socket.send(packet.payload, m_clientSessions[id].endpoint.address,
                              m_clientSessions[id].endpoint.port) == sf::Socket::Done;

            if (packet.hasFlag(Packet::Flag::Reliable)) {
                m_packetBuffer.append(std::move(packet), id);
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

    Packet Server::createPacket(CommandToClient command, bool reliable)
    {
        if (reliable) {
            return createCommandPacket(command, Packet::Flag::Reliable,
                                       ++m_currSequenceNumber);
        }
        else {
            return createCommandPacket(command);
        }
    }

    bool Server::getFromClient(PackagedCommand &package)
    {
        sf::Packet packet;
        if (m_socket.receive(packet, package.endpoint.address, package.endpoint.port) ==
            sf::Socket::Done) {
            package.packet.initFromPacket(packet);
            return true;
        }
        return false;
    }
} // namespace server
