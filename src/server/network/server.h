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

class Server {
	public:
		Server();
		void recievePackets();

		    struct Player {
                    float x = 0, y = 0, z = 0;
                };
                std::array<Player, MAX_CONNECTIONS> players;

		void tick();
		
		void sendPacket(client_id_t client, Packet& packet);
    	void broadcastPacket(Packet &packet);

		const ClientConnector& clients() const;

	private:
		void processPacket(Packet& packet);

		void handleConnectRequest(Packet& packet);
		void handleDisconnect(Packet& packet);
		void handlePlayerPosition(Packet &packet);


    	ClientConnector m_clients;
    	sf::UdpSocket m_socket;
};