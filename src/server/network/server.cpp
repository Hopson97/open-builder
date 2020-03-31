#include "server.h"

#include <common/network/net_constants.h>

Server::Server(int maxConnections)
    : m_clients(maxConnections)
    , m_maxConnections(maxConnections)
    , m_salt(createHandshakeRandom())
{
    m_clientsMap.reserve(maxConnections);

    ENetAddress address{};
    address.host = ENET_HOST_ANY;
    address.port = DEFAULT_PORT;
    mp_host = enet_host_create(&address, maxConnections, 2, 0, 0);
}

Server::~Server()
{
    if (mp_host) {
        enet_host_destroy(mp_host);
        for (auto& session : m_clients) {
            session.disconnect();
        }
    }
}

bool Server::isSetup() const
{
    return mp_host != nullptr;
}

void Server::tick()
{
    assert(mp_host);
    ENetEvent event;
    while (enet_host_service(mp_host, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                std::cout << "Got a connection " << event.peer->incomingPeerID
                          << std::endl;
                addPendingConnection(event.peer);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
            case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
                std::cout << "Got a disconnection " << event.peer->incomingPeerID
                          << std::endl;
                break;

            case ENET_EVENT_TYPE_RECEIVE: {
                ServerPacket packet(event.packet);
                handlePacket(packet, event.peer);
                enet_packet_destroy(event.packet);
            } break;

            default:
                break;
        }
    }
}

void Server::handlePacket(ServerPacket& packet, ENetPeer* peer)
{
    using Cmd = ServerCommand;
    // clang-format off
    switch (packet.command) {
        case Cmd::HandshakePartOne: onHandshakePartOne(packet, peer); break;
        case Cmd::HandshakeResponse: onHandshakeResponse(packet, peer); break;
    }
    // clang-format on
}

void Server::addPendingConnection(ENetPeer* peer)
{
    PendingClientSession session;
    session.connection.peer = peer;
    m_pendingConnections.push_back(session);
}

void Server::onHandshakePartOne(ServerPacket& packet, ENetPeer* peer)
{
    for (auto& pending : m_pendingConnections) {
        if (pending.connection.peer->incomingPeerID == peer->incomingPeerID) {
            pending.salt = packet.salt;
            pending.sendHandshakeChallenge(m_salt);
        }
    }
}

void Server::onHandshakeResponse(ServerPacket& packet, ENetPeer* peer)
{
    for (auto itr = m_pendingConnections.begin(); itr != m_pendingConnections.end();) {
        auto& pending = *itr;
        auto peer = pending.connection.peer;
        if (peer->incomingPeerID == peer->incomingPeerID) {
            u32 salt = itr->salt ^ m_salt;
            if (salt == packet.salt) {
                itr->salt = salt;
                int slot = createClientSession(peer, salt);
                if (slot != -1) {
                    std::cout << "Connection was accepted\n";
                    pending.sendAcceptConnection();
                }
                else {
                    pending.sendRejectConnection("Game Full");
                }
            }
            else {
                std::cout << "Connection was rejected\n";
            }
            itr = m_pendingConnections.erase(itr);
        }
    }
}

int Server::createClientSession(ENetPeer* peer, u32 salt)
{
    for (int i = 0; i < m_clients.size(); i++) {
        if (!m_clients[i].isActive()) {
            std::cout << "Session created!\n";
            m_clients[i].init(peer, salt);
            return true;
        }
    }
    return false;
}
