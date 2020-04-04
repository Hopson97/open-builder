#include "client_session.h"
#include <common/network/net_command.h>

#include "server.h"

void ClientSession::init(ENetPeer* peer, u32 salt, u32 playerId)
{
    m_clientConnection.salt = salt;
    m_salt = salt;
    m_clientConnection.peer = peer;
    m_playerId = playerId;
    m_isActive = true;
}

void ClientSession::disconnect()
{
    if (m_isActive) {
        enet_peer_disconnect(m_clientConnection.peer, 0);
        m_isActive = false;
    }
}

bool ClientSession::verify(u32 salt) const
{
    return salt == m_clientConnection.salt;
}

bool ClientSession::isActive() const
{
    return m_isActive;
}

u32 ClientSession::getPlayerId() const
{
    return m_playerId;
}
