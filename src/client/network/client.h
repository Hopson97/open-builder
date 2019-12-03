#pragma once

#include <SFML/Network/UdpSocket.hpp>
#include <common/network/net_types.h>

class ClientConnection {
  public:
    void connectTo(const sf::IpAddress &address);

  private:
    sf::UdpSocket m_socket;
    Endpoint m_serverEndpoint;
    client_id_t m_clientId = 0;
};


