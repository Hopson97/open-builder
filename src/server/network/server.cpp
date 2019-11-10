#include "server.h"

#include <SFML/Network/Packet.hpp>

#include <common/network/commands.h>
#include <common/network/input_state.h>

#include <ctime>
#include <iostream>
#include <random>
#include <thread>

namespace server {
    Server::Server(int maxConnections)
        : m_clientSessions(maxConnections)
        , m_clientStatuses(maxConnections)
    {
        std::fill(m_clientStatuses.begin(), m_clientStatuses.end(),
                  ClientStatus::Disconnected);

        m_socket.setBlocking(false);
        m_socket.bind(PORT);

        std::cout << "\n\nServer has started!" << std::endl;
        std::cout << "Listening for connections...\n" << std::endl;

        for (auto &entity : m_entities) {
            entity.alive = true;
        }
        m_entities[maxConnections + 1].alive = true;
        m_entities[maxConnections + 1].transform.position = {20, 1, 20};
        m_isRunning = true;
    }

    void Server::run(sf::Time timeout)
    {
        sf::Clock dtClock;
        while (m_isRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(33));

            recievePackets();
            update(dtClock.restart().asSeconds());
            sendPackets();

            if (m_connections == 0) {
                if (m_exitClock.getElapsedTime() > timeout) {
                    m_isRunning = false;
                }
            }
            else {
                m_exitClock.restart();
            }
        }
    }

    bool Server::isRunning() const
    {
        return m_isRunning;
    }

    int Server::findEmptySlot()
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
            switch (package.command) {
                case CommandToServer::KeyInput:
                    handleKeyInput(packet);
                    break;

                case CommandToServer::Connect:
                    handleIncomingConnection(package.address, package.port);
                    break;

                case CommandToServer::Disconnect:
                    handleDisconnect(packet);
                    break;
            }
        }
    }

    void Server::update(float dt)
    {
        // Handle key input
        for (int i = 0; i < m_maxConnections; i++) {
            auto &input = m_clientSessions[i].keyState;
            auto &entity = m_entities[i];
            auto &position = entity.transform.position;
            auto &velocity = entity.velocity;
            auto &rotation = entity.transform.rotation;

            auto isPressed = [input](KeyInput key) {
                return (input & key) == key;
            };

            float speed = 0.8f;
            float s = speed;
            if (isPressed(KeyInput::Forwards)) {
                velocity.x += -glm::cos(glm::radians(rotation.y + 90)) * s;
                velocity.z += -glm::sin(glm::radians(rotation.y + 90)) * s;
            }
            else if (isPressed(KeyInput::Back)) {
                velocity.x += glm::cos(glm::radians(rotation.y + 90)) * speed;
                velocity.z += glm::sin(glm::radians(rotation.y + 90)) * speed;
            }
            if (isPressed(KeyInput::Left)) {
                velocity.x += -glm::cos(glm::radians(rotation.y)) * speed;
                velocity.z += -glm::sin(glm::radians(rotation.y)) * speed;
            }
            else if (isPressed(KeyInput::Right)) {
                velocity.x += glm::cos(glm::radians(rotation.y)) * speed;
                velocity.z += glm::sin(glm::radians(rotation.y)) * speed;
            }
            position += velocity * dt;
            velocity *= 0.85f;
            // entity.transform.rotation.y += 1;
        }

        for (u16 i = m_maxConnections; i < m_entities.size(); i++) {
            m_entities[i].transform.position = {rand() % 64, 0, rand() % 64};
        }
    }

    void Server::sendPackets()
    {
        sf::Packet statePacket;
        statePacket << CommandToClient::WorldState
                    << static_cast<u16>(m_entities.size());
        for (u16 entityId = 0; entityId < m_entities.size(); entityId++) {
            if (m_entities[entityId].alive) {
                auto &transform = m_entities[entityId].transform;
                statePacket << entityId << transform.position.x
                            << transform.position.y << transform.position.z
                            << transform.rotation.x << transform.rotation.y;
            }
        }
        sendToAllClients(statePacket);
    }

    void Server::handleKeyInput(sf::Packet &packet)
    {
        ClientId client;

        packet >> client;
        packet >> m_clientSessions[client].keyState;
        packet >> m_entities[client].transform.rotation.x;
        packet >> m_entities[client].transform.rotation.y;
    }

    bool Server::sendToClient(ClientId id, sf::Packet &packet)
    {
        if (m_clientStatuses[id] == ClientStatus::Connected) {
            return m_socket.send(packet, m_clientSessions[id].address,
                                 m_clientSessions[id].port) == sf::Socket::Done;
        }
        return false;
    }

    void Server::sendToAllClients(sf::Packet &packet)
    {
        for (int i = 0; i < m_maxConnections; i++) {
            sendToClient(i, packet);
        }
    }

    bool Server::getFromClient(PackagedCommand &package)
    {
        if (m_socket.receive(package.packet, package.address, package.port) ==
            sf::Socket::Done) {
            package.packet >> package.command;
            return true;
        }
        return false;
    }

    void Server::handleIncomingConnection(const sf::IpAddress &clientAddress,
                                          Port clientPort)
    {
        std::cout << "Connection request got\n";

        auto sendRejection = [this](ConnectionResult result,
                                    const sf::IpAddress &address, Port port) {
            auto rejectPacket =
                createCommandPacket(CommandToClient::ConnectRequestResult);
            rejectPacket << result;
            m_socket.send(rejectPacket, address, port);
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
            sf::Packet responsePacket;
            responsePacket << CommandToClient::ConnectRequestResult
                           << ConnectionResult::Success
                           << static_cast<ClientId>(slot)
                           << static_cast<u8>(m_maxConnections);

            m_clientStatuses[slot] = ClientStatus::Connected;
            m_clientSessions[slot].address = clientAddress;
            m_clientSessions[slot].port = clientPort;
            m_entities[slot].transform.position = {10, 0, 10};
            m_entities[slot].alive = true;

            m_aliveEntities++;
            m_socket.send(responsePacket, clientAddress, clientPort);

            m_connections++;
            std::cout << "Client Connected slot: " << (int)slot << '\n';

            auto joinPack = createCommandPacket(CommandToClient::PlayerJoin);
            joinPack << static_cast<ClientId>(slot);
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
        ClientId client;
        packet >> client;
        m_clientStatuses[client] = ClientStatus::Disconnected;
        m_entities[client].alive = false;
        m_connections--;
        m_aliveEntities--;
        std::cout << "Client Disonnected slot: " << (int)client << '\n';
        std::cout << std::endl;

        auto joinPack = createCommandPacket(CommandToClient::PlayerLeave);
        joinPack << client;
        sendToAllClients(joinPack);
    }

} // namespace server
