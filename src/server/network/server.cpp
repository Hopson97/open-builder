#include "server.h"

#include <common/debug.h>
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
	if (slot >= 0)
    {
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
