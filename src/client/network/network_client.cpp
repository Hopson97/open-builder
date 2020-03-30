#include "network_client.h"

#include <SFML/System/Clock.hpp>
#include <cassert>
#include <common/network/net_constants.h>
#include <iostream>

const ConnectionResult ConnectionResult::SUCCESS;

ConnectionResult::ConnectionResult(const char* msg)
    : message(msg)
    , success(false)
{
}

NetworkClient::NetworkClient()
{
    // Create the network host
    mp_host = enet_host_create(nullptr, 1, 2, 0, 0);
}
NetworkClient::~NetworkClient()
{
    if (mp_host) {
        disconnect();
        enet_host_destroy(mp_host);
    }
}

ConnectionResult NetworkClient::connectTo(const std::string& ipaddress)
{
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

    // Wait for a connection establishment
    bool connected = [this] {
        sf::Clock clock;
        while (clock.getElapsedTime() < sf::seconds(3)) {
            ENetEvent event;
            while (enet_host_service(mp_host, &event, 0) > 0) {
                if (event.type == ENET_EVENT_TYPE_CONNECT) {
                    return true;
                }
            }
        }
        return false;
    }();
    if (!connected) {
        return "Failed to establish connection with the server.";
    }

    m_connectionState = ConnectionState::Pending;
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

void NetworkClient::disconnect()
{
    assert(mp_host);
    assert(m_serverConnection.peer);
    enet_peer_disconnect(m_serverConnection.peer, 0);
    ENetEvent event;
    while (enet_host_service(mp_host, &event, 3000) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                enet_host_flush(mp_host);
                m_connectionState = ConnectionState::Disconnected;
                return;

            default:
                break;
        }
    }
    enet_peer_reset(m_serverConnection.peer);
}

ConnectionState NetworkClient::getConnnectionState() const
{
    return m_connectionState;
}
