#include "../server_engine.h"

#define AUTHENTICATE_PACKET                                                              \
    auto itr = m_clientsMap.find(peer->incomingPeerID);                                  \
    if (itr == m_clientsMap.end()) {                                                     \
        return;                                                                          \
    }                                                                                    \
    auto& client = m_clients.at(itr->second);                                            \
    if (!client.verify(packet.getSalt())) {                                              \
        return;                                                                          \
    }

void ServerEngine::handlePacket(ServerPacket& packet, ENetPeer* peer)
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

void ServerEngine::onHandshakePartOne(ServerPacket& packet, ENetPeer* peer)
{
    for (auto& pending : m_pendingConnections) {
        if (pending.connection.peer->incomingPeerID == peer->incomingPeerID) {
            pending.salt = packet.getSalt();
            pending.sendHandshakeChallenge(m_salt);
        }
    }
}

void ServerEngine::onHandshakeResponse(ServerPacket& packet, ENetPeer* peer)
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
                    pending.sendAcceptConnection(m_clients[slot].getPlayerId(), m_world);
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

void ServerEngine::onPlayerState(ServerPacket& packet, ENetPeer* peer)
{
    AUTHENTICATE_PACKET
    auto position = packet.read<glm::vec3>();
    auto rotation = packet.read<glm::vec3>();

    auto& player = m_world.findEntity(client.getPlayerId());
    player.lastPosition = player.position;
    player.lastRotation = player.rotation;

    player.position = position;
    player.rotation = rotation;
}

void ServerEngine::handleDisconnection(ENetPeer* peer)
{
    auto itr = m_clientsMap.find(peer->incomingPeerID);
    if (itr != m_clientsMap.end()) {
        auto index = itr->second;
        m_world.removeEntity(m_clients[index].getPlayerId());
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
