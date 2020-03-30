#include "network_client.h"

#include <cassert>
#include <common/network/net_constants.h>
#include <iostream>

const ConnectionResult ConnectionResult::SUCCESS;

ConnectionResult::ConnectionResult(const char* msg)
    : message(msg)
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
    m_serverConnection.peer = enet_host_connect(mp_host, &address, 2, 0);
    if (!m_serverConnection.peer) {
        return "Failed to connect to the server.";
    }
    
    return ConnectionResult::SUCCESS;
}

void NetworkClient::tick()
{
    assert(m_serverConnection.peer);
    assert(mp_host);
    ENetEvent event;
    while (enet_host_service(mp_host, &event, 0) > 0) {
        if (event.type == ENET_EVENT_TYPE_RECEIVE) {
            std::cout << "Got a event " << event.peer->incomingPeerID << std::endl;
            switch (m_connectionState) {
                case ConnectionState::Connected:
                case ConnectionState::Disconnected:
                case ConnectionState::Pending:
                    break;
            }
        }
    }
}
