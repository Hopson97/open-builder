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
        PackagedCommand package;
        while (getFromServer(package)) {
            auto &packet = package.packet;
            if (package.flags == (u8)Packet::Flag::Reliable) {
                auto ack = createCommandPacket(CommandToServer::Acknowledgment);
                ack.payload << package.seq << m_clientId;
                sendToServer(ack.payload);
            }
            switch (package.command) {
                case CommandToClient::WorldState:
                    handleWorldState(packet);
                    break;

                case CommandToClient::ChunkData:
                    handleChunkData(packet);
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

    bool Client::getFromServer(PackagedCommand &package)
    {
        sf::IpAddress address;
        port_t port;
        if (m_socket.receive(package.packet, address, port) ==
            sf::Socket::Done) {
            package.packet >> package.command >> package.flags;
            if (package.flags == (u8)Packet::Flag::Reliable) {
                package.packet >> package.seq;
            }
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

        mp_world.chunks.insert(std::make_pair(position, chunk));
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
