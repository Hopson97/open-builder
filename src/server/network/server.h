#pragma once

#include <array>
#include <common/network/packet.h>
#include <SFML/Network/UdpSocket.hpp>
#include <common/network/net_constants.h>

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

struct Server {
    ClientConnector clients;
    sf::UdpSocket socket;

	void sendPacket(client_id_t client, Packet& packet);
    void broadcastPacket(Packet &packet);
};