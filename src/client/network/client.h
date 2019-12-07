#pragma once

#include <SFML/Network/UdpSocket.hpp>
#include <array>
#include <common/network/net_command.h>
#include <common/network/net_constants.h>
#include <common/network/net_types.h>

#include "../maths.h"

struct Packet;

class ClientConnection {
  public:
    bool connectTo(const sf::IpAddress &address);
    void disconnect();

    bool sendToServer(Packet &packet);

    bool isConnected() const;

    client_id_t getClientId() const;

    sf::UdpSocket socket;

  private:
    Endpoint m_serverEndpoint;
    client_id_t m_clientId = 0;
    bool m_isConnected = false;
};
