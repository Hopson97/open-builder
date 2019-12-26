#include "net_host.h"

#include "../debug.h"
#include "net_constants.h"

#include <SFML/System/Clock.hpp>

namespace {
ENetHost *createHost(const ENetAddress *address, int connections)
{
    return enet_host_create(address, connections, 2, 0, 0);
}

ENetPeer *connectHostTo(ENetHost *host, const std::string &ip)
{
    ENetAddress address{};
    address.port = DEFAULT_PORT;
    if (enet_address_set_host(&address, ip.c_str()) != 0) {
        LOG("Connection", "Failed to create address.");
        return nullptr;
    }

    ENetPeer *peer = enet_host_connect(host, &address, 2, 0);
    if (!peer) {
        LOG("Connection", "Failed to connect to server (Game Full).");
        return nullptr;
    }

    ENetEvent event;
    if (enet_host_service(host, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT) {
        return peer;
    }
    else {
        LOG("Connection", "Failed to connect to the server");
        enet_peer_reset(peer);
        return nullptr;
    }
}

int getPeerIdFromServer(ENetHost *host)
{
    int id = -1;
    sf::Clock test;
    ENetEvent event;
    while (test.getElapsedTime().asSeconds() < 2.0f) {
        enet_host_service(host, &event, 0);
        if (event.type == ENET_EVENT_TYPE_RECEIVE) {
            ClientCommand command;
            sf::Packet packet;
            packet.append(event.packet->data, event.packet->dataLength);
            packet >> command;
            if (command == ClientCommand::PeerId) {
                peer_id_t peerId;
                packet >> peerId;
                id = peerId;
                break;
            }
        }
    }
    return id;
}

ENetPacket *createPacket(sf::Packet &packet, u32 flags)
{
    return enet_packet_create(packet.getData(), packet.getDataSize(), flags);
}
} // namespace

NetworkHost::NetworkHost(std::string &&name)
    : m_name(std::move(name))
{
}

NetworkHost::~NetworkHost()
{
    enet_host_destroy(mp_host);
}

std::optional<ENetPeer *> NetworkHost::createAsClient(const std::string &ip)
{
    mp_host = createHost(0, 1);
    if (!mp_host) {
        LOG(m_name.c_str(), "Error: Failed to create host.");
        return {};
    }

    auto server = connectHostTo(mp_host, ip);
    if (!server) {
        LOG(m_name.c_str(), "Error: Failed to connect to server (Game Full).");
        return {};
    }
    flush();

    int id = getPeerIdFromServer(mp_host);
    if (id == -1) {
        LOG(m_name.c_str(), "Error: Peer ID was not received from server.");
        return {};
    }
    m_peerId = static_cast<peer_id_t>(id);
    return server;
}

bool NetworkHost::createAsServer(int maxConnections)
{
    m_maxConnections = maxConnections;
    ENetAddress address{};
    address.host = ENET_HOST_ANY;
    address.port = DEFAULT_PORT;
    mp_host = createHost(&address, maxConnections);
    return mp_host;
}

void NetworkHost::disconnectFromPeer(ENetPeer *peer)
{
    enet_peer_disconnect(peer, static_cast<u32>(m_peerId));
    ENetEvent event;
    while (enet_host_service(mp_host, &event, 3000) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                flush();
                return;

            default:
                break;
        }
    }
    enet_peer_reset(peer);
}

void NetworkHost::disconnectAllPeers()
{
    int maxConnection = getConnectedPeerCount();
    int connections = maxConnection;
    for (int i = 0; i < maxConnection; i++) {
        enet_peer_disconnect(&mp_host->peers[i], 0);
    }

    ENetEvent event;
    LOG(m_name.c_str(), "Allowing 1 second to disconnect all peers.")
    while (enet_host_service(mp_host, &event, 1000) > 0 && connections > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                connections--;
                break;

            default:
                break;
        }
    }
}

int NetworkHost::getConnectedPeerCount() const
{
    return mp_host->connectedPeers;
}

peer_id_t NetworkHost::getPeerId() const
{
    return m_peerId;
}

int NetworkHost::getMaxConnections() const
{
    return m_maxConnections;
}

bool NetworkHost::sendToPeer(ENetPeer *peer, sf::Packet &packet, u8 channel,
                             u32 flags)
{
    ENetPacket *pkt = createPacket(packet, flags);
    int result = enet_peer_send(peer, channel, pkt);
    flush();
    return result == 0;
}

void NetworkHost::broadcastToPeers(sf::Packet &packet, u8 channel, u32 flags)
{
    ENetPacket *pkt = createPacket(packet, flags);
    enet_host_broadcast(mp_host, channel, pkt);
    flush();
}

void NetworkHost::tick()
{
    ENetEvent event;
    while (enet_host_service(mp_host, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                onPeerConnect(event.peer);
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                onCommandRecieve(event.peer, *event.packet);
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                onPeerDisconnect(event.peer);
                break;

            case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
                onPeerTimeout(event.peer);
                break;

            case ENET_EVENT_TYPE_NONE:
                break;
        }
    }
}

void NetworkHost::onCommandRecieve(ENetPeer *peer, const ENetPacket &enetPacket)
{
    sf::Packet packet;
    packet.append(enetPacket.data, enetPacket.dataLength);
    command_t command;
    packet >> command;
    onCommandRecieve(peer, packet, command);
}

void NetworkHost::flush()
{
    enet_host_flush(mp_host);
}