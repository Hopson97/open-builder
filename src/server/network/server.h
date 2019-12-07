#pragma once

#include <SFML/Network/UdpSocket.hpp>
#include <array>
#include <common/network/net_constants.h>
#include <common/network/packet.h>

/**
 * @brief Provides an interface for client connections
 */
class ClientConnector {
  public:
    /**
     * @brief Tries to connect a client
     * @param endpoint The endpoint of the client trying to connect
     * @return int The client id of the new client, returns -1 if the client did not connect
     */
    int addClient(const Endpoint &endpoint);

    /**
     * @brief Tries to disconnect a client
     * @TODO Authorisation to ensure it is only this client that can diconnect
     * @param id The ID of the client to disconnect
     * @return true A client was disconnected
     * @return false A client was not disconnected
     */
    bool removeClient(client_id_t id);

    /**
     * @brief Gets the endpoint of a client
     * @param id The ID of the client to get the endpoint of
     * @return const Endpoint& The endpoint of the client
     */
    const Endpoint &clientEndpoint(client_id_t id);

    /**
     * @brief Check if the client is connected
     * @param id The ID of the client to check is connected
     * @return true Client is currently connected
     * @return false Client is not connected
     */
    bool clientIsConnected(client_id_t id) const;

    /**
     * @brief Gets the number of clients currently connected
     * @return int Number of connected clients
     */
    int connectedCount() const;

  private:
    std::array<Endpoint, MAX_CONNECTIONS> m_endpoints;
    std::array<bool, MAX_CONNECTIONS> m_isClientConnected{false};
    int m_connectedCount = 0;

    int emptySlot();
};

/**
 * @brief The server, handles the sending and interaction between the server and a client
 */
class Server {
  public:
    sf::UdpSocket socket;
    ClientConnector clients;

  public:
    Server();

    void sendPacket(client_id_t client, Packet &packet);
    void broadcastPacket(Packet &packet);

    /**
     * @brief Try handle a connection request
     * @param packet Connection request packet
     * @return int The client ID of the newly connected client, -1 if they did not connect
     */
    [[nodiscard]] int tryConnectClient(Packet &packet);

    /**
     * @brief Try to disconnect a client
     * 
     * @param packet Disconnection request packet
     * @return int The client id of the disconnected client, -1 if one did not disconnect
     */
    [[nodiscard]] int tryDisconnectClient(Packet &packet);
};