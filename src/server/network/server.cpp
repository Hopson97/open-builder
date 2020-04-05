#include "server.h"

#include <common/debug.h>
#include <common/network/net_constants.h>

#include "../world/server_world.h"

#define AUTHENTICATE_PACKET                                                              \
    auto itr = m_clientsMap.find(peer->incomingPeerID);                                  \
    if (itr == m_clientsMap.end()) {                                                     \
        return;                                                                          \
    }                                                                                    \
    auto& client = m_clients.at(itr->second);                                            \
    if (!client.verify(packet.getSalt())) {                                              \
        return;                                                                          \
    }

Server::Server(int maxConnections, ServerWorld& world)
    : m_host({ENET_HOST_ANY, DEFAULT_PORT}, maxConnections)
    , m_clients(maxConnections)
    , m_maxConnections(maxConnections)
    , m_salt(createHandshakeRandom())
    , mp_world(&world)
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

void Server::broadcastPacket(ServerPacket& packet, int channel, int flags)
{
    for (auto& client : m_clients) {
        if (client.isActive())
            client.sendPacket(packet, channel, flags);
    }
}

void Server::handlePacket(ServerPacket& packet, ENetPeer* peer)
{
    using Cmd = ServerCommand;
    // clang-format off
    switch (packet.command()) {
        case Cmd::HandshakePartOne:     onHandshakePartOne(packet, peer);   break;
        case Cmd::HandshakeResponse:    onHandshakeResponse(packet, peer);  break;

        case Cmd::PlayerState:  onPlayerState(packet, peer);  break;
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
            pending.salt = packet.getSalt();
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
            if (salt == packet.getSalt()) {
                itr->salt = salt;
                int slot = createClientSession(peer, salt);
                if (slot != -1) {
                    pending.sendAcceptConnection(m_clients[slot].getPlayerId(),
                                                 *mp_world);
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

void Server::onPlayerState(ServerPacket& packet, ENetPeer* peer)
{
    AUTHENTICATE_PACKET
    auto position = packet.read<glm::vec3>();
    auto rotation = packet.read<glm::vec3>();

    auto& player = mp_world->findEntity(client.getPlayerId());
    player.lastPosition = player.position;
    player.lastRotation = player.rotation;

    player.position = position;
    player.rotation = rotation;
}

void Server::broadcastEntityStates()
{
    ServerPacket packet(ClientCommand::UpdateEntityStates, m_salt);
    mp_world->serialiseEntities(packet);

    broadcastPacket(packet);
}

void Server::broadcastPlayerJoin(u32 playerId)
{
    ServerPacket packet(ClientCommand::AddEntity, m_salt);
    packet.write((u32)1);
    packet.write(playerId);
    std::cout << "Player join " << playerId << std::endl;
    auto& player = mp_world->findEntity(playerId);
    packet.write(player.position);
    packet.write(player.rotation);

    broadcastToPeers(m_host.handle, packet.get(), 0, ENET_PACKET_FLAG_RELIABLE);
}

void Server::broadcastPlayerLeave(u32 playerId)
{
    ServerPacket packet(ClientCommand::RemoveEntity, m_salt);
    packet.write(playerId);
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
            u32 playerId = mp_world->addEntity();
            m_clients[i].init(peer, salt, playerId);
            m_clientsMap[peer->incomingPeerID] = i;
            broadcastPlayerJoin(playerId);
            return i;
        }
    }
    return -1;
}

void Server::handleDisconnection(ENetPeer* peer)
{
    auto itr = m_clientsMap.find(peer->incomingPeerID);
    if (itr != m_clientsMap.end()) {
        auto index = itr->second;
        mp_world->removeEntity(m_clients[index].getPlayerId());
        m_clients[index].disconnect();
        broadcastPlayerLeave(m_clients[index].getPlayerId());
        m_clientsMap.erase(itr);
    }
    else {
        auto pending = findPendingSession(peer->incomingPeerID);
        if (pending != m_pendingConnections.end()) {
            m_pendingConnections.erase(pending);
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
