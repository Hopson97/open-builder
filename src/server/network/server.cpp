#include "server.h"

#include <common/network/net_constants.h>

Server::Server(int maxConnections)
    : m_host({ENET_HOST_ANY, DEFAULT_PORT}, maxConnections)
    , m_clients(maxConnections)
    , m_maxConnections(maxConnections)
    , m_salt(createHandshakeRandom())
{
    m_clientsMap.reserve(maxConnections);
}

Server::~Server()
{
    for (auto& session : m_clients) {
        session.disconnect();
    }
}

bool Server::isSetup() const
{
    return m_host.handle != nullptr;
}

void Server::tick()
{
    assert(m_host.handle);
    NetEvent event;
    while (m_host.pumpEvent(event)) {
        switch (event.type) {
            case NetEventType::Connection:
                addPendingConnection(event.peer);
                break;

            case NetEventType::Disconnect:
            case NetEventType::Timeout:
                break;

            case NetEventType::Data: {
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
        default: std::cout << "Some other event smh\n"; break;
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
                    pending.sendAcceptConnection();
                }
                else {
                    pending.sendRejectConnection("Game Full");
                }
            }
            else {
            }
            itr = m_pendingConnections.erase(itr);
        }
    }
}

int Server::createClientSession(ENetPeer* peer, u32 salt)
{
    for (int i = 0; i < m_clients.size(); i++) {
        if (!m_clients[i].isActive()) {
            m_clients[i].init(peer, salt);
            return true;
        }
    }
    return false;
}
