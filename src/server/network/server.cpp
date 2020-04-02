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
    broadcastServerShutdown();
    enet_host_flush(m_host.handle);
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
                handleDisconnection(event.peer);
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
    switch (packet.command()) {
        case Cmd::HandshakePartOne:     onHandshakePartOne(packet, peer);   break;
        case Cmd::HandshakeResponse:    onHandshakeResponse(packet, peer);  break;
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
            pending.salt = packet.salt();
            pending.sendHandshakeChallenge(m_salt);
        }
    }
}

void Server::onHandshakeResponse(ServerPacket& packet, ENetPeer* peer)
{
    auto itr = findPendingSession(peer->incomingPeerID);
    if (itr != m_pendingConnections.end()) {
        auto& pending = *itr;
        auto thepeer = pending.connection.peer;
        if (thepeer->incomingPeerID == peer->incomingPeerID) {
            u32 salt = itr->salt ^ m_salt;
            if (salt == packet.salt()) {
                itr->salt = salt;
                int slot = createClientSession(peer, salt);
                if (slot != -1) {
                    pending.sendAcceptConnection();
                    broadcastPlayerJoin();
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

void Server::broadcastPlayerJoin()
{
    ServerPacket packet(ClientCommand::PlayerJoined, m_salt);
    broadcastToPeers(m_host.handle, packet.get(), 0, ENET_PACKET_FLAG_RELIABLE);
}

void Server::broadcastPlayerLeave()
{
    ServerPacket packet(ClientCommand::PlayerLeave, m_salt);
    broadcastToPeers(m_host.handle, packet.get(), 0, ENET_PACKET_FLAG_RELIABLE);
}

void Server::broadcastServerShutdown()
{
    ServerPacket packet(ClientCommand::ForceExitGame, m_salt);
    packet.write(std::string("Host has exited the game."));
    broadcastToPeers(m_host.handle, packet.get(), 0, ENET_PACKET_FLAG_RELIABLE);
}

int Server::createClientSession(ENetPeer* peer, u32 salt)
{
    for (unsigned i = 0; i < m_clients.size(); i++) {
        if (!m_clients[i].isActive()) {
            m_clients[i].init(peer, salt);
            m_clientsMap[peer->incomingPeerID] = i;
            return true;
        }
    }
    return false;
}

void Server::handleDisconnection(ENetPeer* peer)
{
    auto itr = m_clientsMap.find(peer->incomingPeerID);
    if (itr != m_clientsMap.end()) {
        auto index = itr->second;
        m_clients[index].disconnect();
        m_clientsMap.erase(itr);
        broadcastPlayerLeave();
    }
    else {
        auto itr = findPendingSession(peer->incomingPeerID);
        if (itr != m_pendingConnections.end()) {
            m_pendingConnections.erase(itr);
        }
    }
}

std::vector<PendingClientSession>::iterator Server::findPendingSession(u32 peerId)
{
    for (auto itr = m_pendingConnections.begin(); itr != m_pendingConnections.end();) {
        auto thepeer = itr->connection.peer;
        if (thepeer->incomingPeerID == peerId) {
            return itr;
        }
    }
    return m_pendingConnections.end();
}
