#include "enet.h"

#include <cassert>

#include "net_constants.h"

const ClientConnectionResult ClientConnectionResult::SUCCESS;

ClientConnectionResult::ClientConnectionResult(const char* msg)
    : message(msg)
    , success(false)
{
}



ENetPacket* createPacket(const sf::Packet& packet, u32 flags)
{
    return enet_packet_create(packet.getData(), packet.getDataSize(), flags);
}

void Connection::send(const sf::Packet& packet, int channel, u32 flags)
{
    assert(peer);
    auto enetPacket = createPacket(packet, flags);
    enet_peer_send(peer, channel, enetPacket);
}



ClientConnectionResult connectEnetClientTo(ENetHost* host, Connection& serverConnection,
                                           const char* ipAddress)
{
    // Create address for the client to connect to
    ENetAddress address{};
    address.port = DEFAULT_PORT;
    if (enet_address_set_host(&address, ipAddress) != 0) {
        return "Failed to create address.";
    }

    // Connect to the server
    serverConnection.peer = enet_host_connect(host, &address, 2, 0);
    if (!serverConnection.peer) {
        return "Failed to connect to the server.";
    }

    // Wait for a connection establishment
    bool connected = [&host] {
            ENetEvent event;
            while (enet_host_service(host, &event, 2000) > 0) {
                if (event.type == ENET_EVENT_TYPE_RECEIVE) {
                    enet_packet_destroy(event.packet);
                }
                else if (event.type == ENET_EVENT_TYPE_CONNECT) {
                    return true;
                
            }
        }
        return false;
    }();
    if (!connected) {
        return "Failed to establish connection with the server.";
    }
    return ClientConnectionResult::SUCCESS;
}

bool disconnectEnetClient(ENetHost* host, Connection& serverConnection)
{
    enet_peer_disconnect(serverConnection.peer, 0);
    ENetEvent event;
    while (enet_host_service(host, &event, 2000) > 0) {
        if (event.type == ENET_EVENT_TYPE_RECEIVE) {
            enet_packet_destroy(event.packet);
        }
        else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
            enet_host_flush(host);
            return true;
        }
    }
    enet_peer_reset(serverConnection.peer);
}

void broadcastToPeers(ENetHost* host, u8 channel, sf::Packet& packet, u32 flags)
{
    auto enetpacket = createPacket(packet, flags);
    enet_host_broadcast(host, channel, enetpacket);
}

