#pragma once

#include <SFML/Network/UdpSocket.hpp>
#include <common/network/net_types.h>

struct Packet;

class ClientConnection {
  public:
    bool connectTo(const sf::IpAddress &address);
    void disconnect();

    bool sendToServer(Packet &packet);

    bool isConnected() const;

  private:
    sf::UdpSocket m_socket;
    Endpoint m_serverEndpoint;
    client_id_t m_clientId = 0;
    bool connected = false;
};
