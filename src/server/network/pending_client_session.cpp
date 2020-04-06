#include "client_session.h"
#include <common/network/net_command.h>

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

void PendingClientSession::sendGameData(const ServerWorld& world)
{
    ServerPacket outgoing(ClientCommand::GameData, salt);

    // Send game data about voxels
    auto& voxels = world.getVoxelData().getVoxelData();
    outgoing.write(static_cast<u16>(voxels.size()));
    std::cout << "sending " << voxels.size() << " voxels\n";
    for (const VoxelData& voxel : voxels) {
        outgoing.write(voxel);
    }

    // Send current world entities
    world.serialiseEntities(outgoing);

    connection.send(outgoing.get(), 0, ENET_PACKET_FLAG_RELIABLE);
}

void PendingClientSession::sendAcceptConnection(u32 playerId)
{
    // 1 meaning accept
    ServerPacket outgoing(ClientCommand::ConnectionAcceptance, salt);
    outgoing.write((u8)1);

    // Send the player ID
    outgoing.write(playerId);

    connection.send(outgoing.get(), 0, ENET_PACKET_FLAG_RELIABLE);
}
