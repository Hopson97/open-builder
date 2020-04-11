#include "enet.h"

#include <cassert>
#include <ctime>
#include <limits>
#include <random>

#include "net_constants.h"

const ClientConnectionResult ClientConnectionResult::SUCCESS;

namespace {
    ENetPacket* createPacket(const sf::Packet& packet, u32 flags)
    {
        return enet_packet_create(packet.getData(), packet.getDataSize(), flags);
    }
} // namespace

ClientConnectionResult::ClientConnectionResult(const char* msg)
    : message(msg)
    , success(false)
{
}

/**
    Connection functions
*/
void Connection::send(const sf::Packet& packet, int channel, u32 flags)
{
    assert(peer);
    auto enetPacket = createPacket(packet, flags);
    enet_peer_send(peer, channel, enetPacket);
}

NetHost::NetHost()
{
    handle = enet_host_create(nullptr, 1, 2, 0, 0);
}

/**
    Net Host functions
*/
NetHost::NetHost(const ENetAddress& address, int maximumConnections) noexcept
{
    handle = enet_host_create(&address, maximumConnections, 2, 0, 0);
    if (!handle) {
        std::cout << "Failed to create server\n";
    }
}

NetHost::~NetHost() noexcept
{
    if (handle) {
        enet_host_destroy(handle);
    }
}

NetHost::NetHost(NetHost&& other) noexcept
    : handle(other.handle)
{
    enet_host_destroy(other.handle);
    other.handle = nullptr;
}

NetHost& NetHost::operator=(NetHost&& other) noexcept
{
    enet_host_destroy(other.handle);
    other.handle = nullptr;
    return *this;
}

bool NetHost::pumpEvent(NetEvent& event)
{
    if (enet_host_service(handle, &event.data, 0)) {
        event.type = static_cast<NetEventType>(event.data.type);
        event.packet = event.data.packet;
        event.peer = event.data.peer;
        return true;
    }
    return false;
}

/**
    Network helper functions
*/
ClientConnectionResult connectEnetClientTo(ENetHost* host, Connection& serverConnection,
                                           const char* ipAddress)
{
    if (!host) {
        return "Failed to create the host.";
    }
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
    return false;
}

void broadcastToPeers(ENetHost* host, const sf::Packet& packet, u8 channel, u32 flags)
{
    auto enetpacket = createPacket(packet, flags);
    enet_host_broadcast(host, channel, enetpacket);
}

u32 createHandshakeRandom()
{
    std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_int_distribution<u32> dist(0, 4294967290);
    return dist(rng);
}
