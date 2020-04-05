#include "client_session.h"
#include <common/network/net_command.h>

#include "server.h"
#include "../world/server_world.h"

void PendingClientSession::sendHandshakeChallenge(u32 serversalt)
{
    ServerPacket outgoing(ClientCommand::HandshakeChallenge, salt);
    outgoing.write(serversalt);
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

void PendingClientSession::sendAcceptConnection(u32 playerId, ServerWorld& world)
{
    // 1 meaning accept
    ServerPacket outgoing(ClientCommand::ConnectionAcceptance, salt);
    outgoing.write((u8)1);

    // Send ititial player state
    std::cout << playerId << std::endl;
    outgoing.write(playerId);

    // Send entities
    world.serialiseEntities(outgoing);


    connection.send(outgoing.get(), 0, ENET_PACKET_FLAG_RELIABLE);
}
