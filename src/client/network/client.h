#pragma once

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/UdpSocket.hpp>

#include <common/network/commands.h>
#include <common/network/input_state.h>
#include <common/network/packet.h>

#include "../world/entity.h"

namespace client {
    class World;
    class Client final {
      public:
        Client(World &world);
        bool connect(const sf::IpAddress &address);
        void disconnect();

        void sendInput(input_t input, const glm::vec3 &rotation);
        void update();

        bool isConnected() const;

        client_id_t getClientId() const;
        u8 getMaxPlayers() const;

      private:
        struct PackagedCommand {
            sf::Packet packet;
            CommandToClient command;

            u8 flags;
            u32 seq;
        };

        bool sendToServer(sf::Packet &packet);
        bool getFromServer(PackagedCommand &package);

        void handleWorldState(sf::Packet &packet);
        void handleChunkData(sf::Packet &packet);
        void handlePlayerJoin(sf::Packet &packet);
        void handlePlayerLeave(sf::Packet &packet);

        struct {
            sf::IpAddress address;
            port_t port;
        } m_server;

        sf::UdpSocket m_socket;
        client_id_t m_clientId;
        input_t m_inputState = 0;
        bool m_isConnected = false;

        World &mp_world;

        u8 m_serverMaxPlayers = 0;
    };
} // namespace client
