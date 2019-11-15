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
                    handleIncomingConnection(package.address, package.port);
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
                m_socket.send(packet.payload, m_clientSessions[id].address,
                              m_clientSessions[id].port) == sf::Socket::Done;

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
        if (m_socket.receive(packet, package.address, package.port) ==
            sf::Socket::Done) {
            package.packet.initFromPacket(packet);
            return true;
        }
        return false;
    }

    void Server::handleIncomingConnection(const sf::IpAddress &clientAddress,
                                          port_t clientPort)
    {
        std::cout << "Connection request got\n";

        auto sendRejection = [this](ConnectionResult result,
                                    const sf::IpAddress &address, port_t port) {
            auto rejectPacket =
                createCommandPacket(CommandToClient::ConnectRequestResult);
            rejectPacket.payload << result;
            m_socket.send(rejectPacket.payload, address, port);
        };

        // This makes sure there are not any duplicated connections
        for (const auto &endpoint : m_clientSessions) {
            if (clientAddress.toInteger() == endpoint.address.toInteger() &&
                clientPort == endpoint.port) {
                return;
            }
        }

        if (m_connections < m_maxConnections) {
            auto slot = findEmptySlot();
            if (slot < 0) {
                sendRejection(ConnectionResult::GameFull, clientAddress,
                              clientPort);
            }
            // Connection can be made
            auto responsePacket =
                createCommandPacket(CommandToClient::ConnectRequestResult);
            responsePacket.payload << ConnectionResult::Success
                                   << static_cast<peer_id_t>(slot)
                                   << static_cast<u8>(m_maxConnections);

            m_clientStatuses[slot] = ClientStatus::Connected;
            m_clientSessions[slot].address = clientAddress;
            m_clientSessions[slot].port = clientPort;
            m_clientSessions[slot].p_entity->position.y = 70.0f;
            m_clientSessions[slot].p_entity->isAlive = true;
            m_clientSessions[slot].p_entity->speed = 16.0f;

            m_aliveEntities++;
            m_socket.send(responsePacket.payload, clientAddress, clientPort);

            m_connections++;
            std::cout << "Client Connected slot: " << (int)slot << '\n';

            auto joinPack = createPacket(CommandToClient::PlayerJoin, false);
            joinPack.payload << static_cast<peer_id_t>(slot);
            sendToAllClients(joinPack);
        }
        else {
            sendRejection(ConnectionResult::GameFull, clientAddress,
                          clientPort);
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

        auto joinPack = createPacket(CommandToClient::PlayerLeave, false);
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
