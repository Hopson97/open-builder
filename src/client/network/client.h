#pragma once

#include <SFML/Network/UdpSocket.hpp>
#include <array>
#include <common/network/net_command.h>
#include <common/network/net_constants.h>
#include <common/network/net_types.h>

#include "../maths.h"

struct Packet;

/**
 * @brief Provides an interface for connection between the client and the server
 */
class ClientConnection {
  public:
    sf::UdpSocket socket;

  public:
    bool isConnected() const;
    bool sendPacketToServer(Packet &packet);
    bool connectTo(const sf::IpAddress &address);
    void disconnect();
    client_id_t getClientId() const;

  private:
    Endpoint m_serverEndpoint;
    client_id_t m_clientId = 0;
    bool m_isConnected = false;
};
