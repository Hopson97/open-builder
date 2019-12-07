#include "server.h"

#include <common/debug.h>
#include <common/network/net_command.h>
#include <iostream>

int ClientConnector::emptySlot()
{
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (!m_isClientConnected[i]) {
            return i;
        }
    }
    return -1;
}

int ClientConnector::addClient(const Endpoint &endpoint)
{
    int slot = emptySlot();
    if (slot >= 0) {
        LOGVAR("Adding client into slot: ", slot)
        m_endpoints[slot] = endpoint;
        m_isClientConnected[slot] = true;
        m_connectedCount++;
    }
    return slot;
}

bool ClientConnector::removeClient(client_id_t id)
{
    if (m_isClientConnected[id]) {
        m_isClientConnected[id] = false;
        m_connectedCount--;
        return true;
    }
    else {
        return false;
    }
}

const Endpoint &ClientConnector::clientEndpoint(client_id_t id)
{
    return m_endpoints[id];
}

bool ClientConnector::clientIsConnected(client_id_t id) const
{
    return m_isClientConnected[id];
}

int ClientConnector::connectedCount() const
{
    return m_connectedCount;
}

//
//	Server
//
Server::Server()
{

    socket.setBlocking(false);
    socket.bind(DEFAULT_PORT);
}

void Server::sendPacket(client_id_t client, Packet &packet)
{
    if (clients.clientIsConnected(client)) {
        auto &endpoint = clients.clientEndpoint(client);
        socket.send(packet.data, endpoint.address, endpoint.port);
    }
}

void Server::broadcastPacket(Packet &packet)
{
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        sendPacket(i, packet);
    }
}

int Server::tryConnectClient(Packet &packet)
{
    int slot = clients.addClient(packet.endpoint);
    if (slot >= 0) {
        // Send connection acceptance to the connecting client
        auto response = makePacket(ClientCommand::AcceptConnection);
        response.data << static_cast<client_id_t>(slot);
        sendPacket(slot, response);

        // Tell all players that a player has joined
        auto broadcast = makePacket(ClientCommand::PlayerJoin);
        broadcast.data << static_cast<client_id_t>(slot);
        broadcastPacket(broadcast);
        return slot;
    }
    else {
        auto response = makePacket(ClientCommand::RejectConnection);
        socket.send(response.data, response.endpoint.address,
                      response.endpoint.port);
        return -1;
    }
}

int Server::tryDisconnectClient(Packet &packet)
{
    LOG("Server: Disconnect request got")
    client_id_t id = 0;
    packet.data >> id;
    if (clients.removeClient(id)) {

        auto broadcast = makePacket(ClientCommand::PlayerLeave);
        broadcast.data << static_cast<client_id_t>(id);
        broadcastPacket(broadcast);
        return id;
    }
    else {
        return -1;
    }
}
