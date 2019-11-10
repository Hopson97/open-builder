#include "client.h"

#include <common/network/commands.h>
#include <common/network/constants.h>
#include <iostream>
#include <thread>

namespace client {
    Client::Client(EntityArray &entites)
        : mp_entities(entites)
    {
    }

    void Client::sendInput(Input input, const glm::vec3 &rotation)
    {
        auto inputStatePacket = createCommandPacket(CommandToServer::KeyInput);
        inputStatePacket << m_clientId << input << rotation.x << rotation.y;
        sendToServer(inputStatePacket);
    }

    void Client::update()
    {
        PackagedCommand package;
        while (getFromServer(package)) {
            auto &packet = package.packet;
            switch (package.command) {
                case CommandToClient::WorldState:
                    handleWorldState(packet);
                    break;

                case CommandToClient::PlayerJoin:
                    handlePlayerJoin(packet);
                    break;

                case CommandToClient::PlayerLeave:
                    handlePlayerLeave(packet);
                    break;

                case CommandToClient::ConnectRequestResult:
                    break;
            }
        }
    }

    bool Client::isConnected() const
    {
        return m_isConnected;
    }

    ClientId Client::getClientId() const
    {
        return m_clientId;
    }

    u8 Client::getMaxPlayers() const
    {
        return m_serverMaxPlayers;
    }

    bool Client::connect(const sf::IpAddress &address)
    {
        m_serverIpAddress = address;
        auto connectRequest = createCommandPacket(CommandToServer::Connect);
        if (sendToServer(connectRequest)) {
            PackagedCommand response;
            if (getFromServer(response)) {
                ConnectionResult result;
                response.packet >> result;
                switch (result) {
                    case ConnectionResult::Success:
                        response.packet >> m_clientId;
                        std::cout << "Connected, ID: " << (int)m_clientId
                                  << std::endl;
                        m_isConnected = true;
                        m_socket.setBlocking(false);
                        response.packet >> m_serverMaxPlayers;
                        return true;

                    case ConnectionResult::GameFull:
                        std::cout << "Could not connect, game is full.\n";
                        break;

                    case ConnectionResult::DuplicateId:
                        std::cout
                            << "Unable to join on same connection and port\n";
                        break;
                }
            }
            else {
                std::cout << "Unable to connect." << std::endl;
            }
        }
        return false;
    }

    void Client::disconnect()
    {
        auto disconnectRequest =
            createCommandPacket(CommandToServer::Disconnect);
        disconnectRequest << m_clientId;
        if (sendToServer(disconnectRequest)) {
            m_isConnected = false;
        }
    }

    bool Client::sendToServer(sf::Packet &packet)
    {
        return m_socket.send(packet, m_serverIpAddress, PORT) ==
               sf::Socket::Done;
    }

    bool Client::getFromServer(PackagedCommand &package)
    {
        sf::IpAddress address;
        Port port;
        if (m_socket.receive(package.packet, address, port) ==
            sf::Socket::Done) {
            package.packet >> package.command;
            return true;
        }
        return false;
    }

    void Client::handleWorldState(sf::Packet &packet)
    {
        u16 count;
        packet >> count;
        for (unsigned i = 0; i < count; i++) {
            EntityId id;
            packet >> id;
            auto &entity = mp_entities[id];
            auto &transform = entity.transform;

            entity.alive = true;
            packet >> transform.position.x;
            packet >> transform.position.y;
            packet >> transform.position.z;
            float rotx, roty;
            packet >> rotx;
            packet >> roty;

            if (id != m_clientId) {
                transform.rotation.x = rotx;
                transform.rotation.y = roty;
            }
        }
    }

    void Client::handlePlayerJoin(sf::Packet &packet)
    {
        ClientId id;
        packet >> id;
        mp_entities[id].alive = true;

        std::cout << "Player has joined: " << (int)id << std::endl;
    }

    void Client::handlePlayerLeave(sf::Packet &packet)
    {
        ClientId id;
        packet >> id;
        mp_entities[id].alive = false;

        std::cout << "Player has left: " << (int)id << std::endl;
    }
} // namespace client
