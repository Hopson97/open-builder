#include "client_session.h"
#include <common/network/net_command.h>

void PendingClientSession::sendHandshakeChallenge(u32 serversalt)
{
    auto outgoing = createPacket(ClientCommand::HandshakeChallenge, salt);
    outgoing << serversalt;
    connection.send(outgoing, 0, ENET_PACKET_FLAG_RELIABLE);
}

void PendingClientSession::sendAcceptConnection()
{
    auto outgoing = createPacket(ClientCommand::ConnectionAcceptance, salt);

    // '1' meaning accept
    outgoing << (u8)1;
    connection.send(outgoing, 0, ENET_PACKET_FLAG_RELIABLE);
}

void PendingClientSession::sendRejectConnection(const char* reason)
{
    auto outgoing = createPacket(ClientCommand::ConnectionAcceptance, salt);

    // '0' meaning reject
    outgoing << (u8)0 << std::string(reason);
    connection.send(outgoing, 0, ENET_PACKET_FLAG_RELIABLE);
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
