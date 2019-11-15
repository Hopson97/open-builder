#include "client.h"

#include <common/constants.h>
#include <common/network/commands.h>
#include <iostream>
#include <thread>

#include "../world/world.h"

namespace client {
    Client::Client(World &world)
        : mp_world(world)
    {
    }

    void Client::sendInput(input_t input, const glm::vec3 &rotation)
    {
        auto inputStatePacket =
            createCommandPacket(CommandToServer::PlayerInput);
        inputStatePacket.payload << m_clientId << input << rotation.x
                                 << rotation.y;
        sendToServer(inputStatePacket.payload);
    }

    void Client::update()
    {
        Packet packet;
        while (getFromServer(packet)) {
            // auto &packet = packet.payload;
            if (packet.hasFlag(Packet::Flag::Reliable)) {
                auto acknowledgePacket =
                    createCommandPacket(CommandToServer::Acknowledgment);
                acknowledgePacket.payload << packet.sequenceNumber
                                          << m_clientId;
                sendToServer(acknowledgePacket.payload);
            }
            switch (static_cast<CommandToClient>(packet.command)) {
                case CommandToClient::WorldState:
                    handleWorldState(packet.payload);
                    break;

                case CommandToClient::ChunkData:
                    handleChunkData(packet.payload);
                    break;

                case CommandToClient::PlayerJoin:
                    handlePlayerJoin(packet.payload);
                    break;

                case CommandToClient::PlayerLeave:
                    handlePlayerLeave(packet.payload);
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

    client_id_t Client::getClientId() const
    {
        return m_clientId;
    }

    u8 Client::getMaxPlayers() const
    {
        return m_serverMaxPlayers;
    }

    bool Client::connect(const sf::IpAddress &address)
    {
        m_server.address = address;
        auto connectRequest = createCommandPacket(CommandToServer::Connect);
        if (sendToServer(connectRequest.payload)) {
            Packet response;
            if (getFromServer(response)) {
                ConnectionResult result;
                response.payload >> result;
                switch (result) {
                    case ConnectionResult::Success:
                        response.payload >> m_clientId;
                        std::cout << "Connected, ID: " << (int)m_clientId
                                  << std::endl;
                        m_isConnected = true;
                        m_socket.setBlocking(false);
                        response.payload >> m_serverMaxPlayers;
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
        disconnectRequest.payload << m_clientId;
        if (sendToServer(disconnectRequest.payload)) {
            m_isConnected = false;
        }
    }

    bool Client::sendToServer(sf::Packet &packet)
    {
        return m_socket.send(packet, m_server.address, PORT) ==
               sf::Socket::Done;
    }

    bool Client::getFromServer(Packet &package)
    {
        sf::Packet packet;
        sf::IpAddress address;
        port_t port;
        if (m_socket.receive(packet, address, port) == sf::Socket::Done) {
            package.initFromPacket(packet);
            return true;
        }
        return false;
    }

    void Client::handleWorldState(sf::Packet &packet)
    {
        u16 count;
        packet >> count;
        for (unsigned i = 0; i < count; i++) {
            entityid_t id;
            packet >> id;
            auto &entity = mp_world.entities[id];
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

    void Client::handleChunkData(sf::Packet &packet)
    {
        ChunkPosition position;
        packet >> position.x >> position.y >> position.z;
        Chunk chunk(position);
        packet >> chunk;
        chunk.flag = Chunk::Flag::NeedsNewMesh;

        mp_world.chunks.try_emplace(position, std::move(chunk));
    }

    void Client::handlePlayerJoin(sf::Packet &packet)
    {
        client_id_t id;
        packet >> id;
        mp_world.entities[id].alive = true;

        std::cout << "Player has joined: " << (int)id << std::endl;
    }

    void Client::handlePlayerLeave(sf::Packet &packet)
    {
        client_id_t id;
        packet >> id;
        mp_world.entities[id].alive = false;

        std::cout << "Player has left: " << (int)id << std::endl;
    }
} // namespace client
