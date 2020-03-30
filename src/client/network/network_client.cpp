#include "network_client.h"

#include <cassert>
#include <common/network/net_constants.h>
#include <iostream>

const ConnectionResult ConnectionResult::SUCCESS;

ConnectionResult::ConnectionResult(const char* message)
    : message(message)
    , success(false)
{
}

ConnectionResult NetworkClient::connectTo(const std::string& ipaddress)
{
    // Create the network host
    mp_host = enet_host_create(nullptr, 1, 2, 0, 0);
    if (!mp_host) {
        return "Failed to create the host.";
    }

    // Create address for the client to connect to
    ENetAddress address{};
    address.port = DEFAULT_PORT;
    if (enet_address_set_host(&address, ipaddress.c_str()) != 0) {
        return "Failed to create address.";
    }

    // Connect to the server
    ENetPeer* server = enet_host_connect(mp_host, &address, 2, 0);
    if (!server) {
        return "Failed to connect to the server.";
    }

    std::cout << "yay\n";
    return ConnectionResult::SUCCESS;
}

void NetworkClient::tick()
{
    assert(m_serverConnection.peer);
    assert(mp_host);
    ENetEvent event;
    while (enet_host_service(mp_host, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                std::cout << "Got a event " << event.peer->incomingPeerID << std::endl;
                break;

            default:
                break;
        }
    }
}
