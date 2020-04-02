#include "client_session.h"
#include <common/network/net_command.h>

#include "server.h"

void PendingClientSession::sendHandshakeChallenge(u32 serversalt)
{
    ServerPacket outgoing(ClientCommand::HandshakeChallenge, salt);
    outgoing.write(serversalt);
    connection.send(outgoing.get(), 0, ENET_PACKET_FLAG_RELIABLE);
}

void PendingClientSession::sendAcceptConnection()
{
    // 1 meaning accept
    ServerPacket outgoing(ClientCommand::ConnectionAcceptance, salt);
    outgoing.write((u8)1);
    connection.send(outgoing.get(), 0, ENET_PACKET_FLAG_RELIABLE);
}

void PendingClientSession::sendRejectConnection(const char* reason)
{
    // 0 meaning accept
    ServerPacket outgoing(ClientCommand::ConnectionAcceptance, salt);
    outgoing.write((u8)0);
    outgoing.write(std::string(reason));

    connection.send(outgoing.get(), 0, ENET_PACKET_FLAG_RELIABLE);
}

void ClientSession::init(ENetPeer* peer, u32 salt)
{
    m_clientConnection.salt = salt;
    m_salt = salt;
    m_clientConnection.peer = peer;
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
