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
    m_socket.setBlocking(false);
    m_socket.bind(DEFAULT_PORT);
}

void Server::recievePackets()
{
    Packet incoming;
    Endpoint endpoint;
    while (receivePacket(m_socket, incoming)) {
        processPacket(incoming);
    }
}

void Server::tick()
{
    auto packet = makePacket(ClientCommand::Snapshot);
    auto count = static_cast<u16>(m_clients.connectedCount());
    packet.data << count;

    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (m_clients.clientIsConnected(i)) {
            packet.data << static_cast<client_id_t>(i);
            packet.data << players[i].x << players[i].y << players[i].z;
        }
    }
    broadcastPacket(packet);
}

void Server::sendPacket(client_id_t client, Packet &packet)
{
    if (m_clients.clientIsConnected(client)) {
        auto &endpoint = m_clients.clientEndpoint(client);
        m_socket.send(packet.data, endpoint.address, endpoint.port);
    }
}

void Server::broadcastPacket(Packet &packet)
{
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        sendPacket(i, packet);
    }
}

const ClientConnector &Server::clients() const
{
    return m_clients;
}

void Server::processPacket(Packet &packet)
{
    auto command = static_cast<ServerCommand>(packet.command);
    switch (command) {
        case ServerCommand::Connect:
            handleConnectRequest(packet);
            break;

        case ServerCommand::ConnectionChallengeResponse:
            break;

        case ServerCommand::Disconnect:
            handleDisconnect(packet);
            break;

        case ServerCommand::PlayerPosition:
            handlePlayerPosition(packet);
            break;

        default:
            break;
    }
}

void Server::handleConnectRequest(Packet &packet)
{
    LOG("Server: Connection request got")
    int slot = m_clients.addClient(packet.endpoint);
    if (slot >= 0) {
        // Send connection acceptance to the connecting client
        auto packet = makePacket(ClientCommand::AcceptConnection);
        packet.data << static_cast<client_id_t>(slot);
        sendPacket(slot, packet);
        std::cout << "Packet sent\n";

        // Tell all players that a player has joined
        auto broadcast = makePacket(ClientCommand::PlayerJoin);
        broadcast.data << static_cast<client_id_t>(slot);
        broadcastPacket(broadcast);
    }
    else {
        auto packet = makePacket(ClientCommand::RejectConnection);
        m_socket.send(packet.data, packet.endpoint.address,
                      packet.endpoint.port);
    }
}

void Server::handleDisconnect(Packet &packet)
{
    LOG("Server: Disconnect request got")
    client_id_t id = 0;
    packet.data >> id;
    m_clients.removeClient(id);

    auto broadcast = makePacket(ClientCommand::PlayerJoin);
    broadcast.data << static_cast<client_id_t>(id);
    broadcastPacket(broadcast);
}

void Server::handlePlayerPosition(Packet &packet)
{
    client_id_t id = 0;
    packet.data >> id;

    if (m_clients.clientIsConnected(id)) {
        Player *player = &players[id];
        packet.data >> player->x >> player->y >> player->z;
    }
}
