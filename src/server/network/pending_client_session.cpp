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
