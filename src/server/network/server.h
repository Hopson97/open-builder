#pragma once

#include <SFML/Network/UdpSocket.hpp>
#include <array>
#include <common/network/net_constants.h>
#include <common/network/packet.h>

class ClientConnector {
  public:
    int addClient(const Endpoint &endpoint);
    bool removeClient(client_id_t id);

    const Endpoint &clientEndpoint(client_id_t id);
    bool clientIsConnected(client_id_t id) const;
    int connectedCount() const;

  private:
    std::array<Endpoint, MAX_CONNECTIONS> m_endpoints;
    std::array<bool, MAX_CONNECTIONS> m_isClientConnected{false};
    int m_connectedCount = 0;

    int emptySlot();
};

class Server {
  public:
    sf::UdpSocket socket;
    ClientConnector clients;

  public:
    Server();

    void sendPacket(client_id_t client, Packet &packet);
    void broadcastPacket(Packet &packet);

    [[no_discard]] int handleConnectRequest(Packet &packet);
    [[no_discard]] int handleDisconnect(Packet &packet);
};