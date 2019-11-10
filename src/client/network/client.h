#pragma once

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/UdpSocket.hpp>

#include <common/network/commands.h>
#include <common/network/input_state.h>

#include "../game/entity.h"

namespace client {
    class Client final {
      public:
        Client(EntityArray &entites);
        bool connect(const sf::IpAddress &address);
        void disconnect();

        void sendInput(Input input, const glm::vec3 &rotation);
        void update();

        bool isConnected() const;

        ClientId getClientId() const;
        u8 getMaxPlayers() const;

      private:
        struct PackagedCommand {
            sf::Packet packet;
            CommandToClient command;
        };

        bool sendToServer(sf::Packet &packet);
        bool getFromServer(PackagedCommand &package);

        void handleWorldState(sf::Packet &packet);
        void handlePlayerJoin(sf::Packet &packet);
        void handlePlayerLeave(sf::Packet &packet);

        sf::UdpSocket m_socket;
        sf::IpAddress m_serverIpAddress;
        Port m_serverPort;
        ClientId m_clientId;
        Input m_inputState = 0;
        bool m_isConnected = false;

        u8 m_serverMaxPlayers = 0;

        EntityArray &mp_entities;
    };
} // namespace client
