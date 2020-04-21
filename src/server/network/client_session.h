#pragma once

#include <common/network/enet.h>
#include <common/network/packet.h>
#include <common/world/coordinate.h>
#include <unordered_set>

struct VoxelUpdate;
class ServerWorld;
class Chunk;

struct PendingClientSession {
    Connection connection;
    u32 salt = 0;

    void sendHandshakeChallenge(u32 serversalt);
    void sendAcceptConnection(u32 playerId);
    void sendRejectConnection(const char* reason);

    void sendGameData(const ServerWorld& world);
};

class ClientSession {
  public:
    void init(ENetPeer* peer, u32 salt, u32 playerId);
    void disconnect();

    void sendPacket(const ServerPacket& packet, u32 channel = 0, u32 flags = 0);

    void tick(ServerWorld& world);

    bool verify(u32 salt) const;
    bool isActive() const;
    u32 getPlayerId() const;

    void sendAddChunk(const Chunk& chunk);
    void sendPlayerSpawnPoint(const glm::vec3& position);
    void sendVoxelUpdate(const VoxelUpdate& update);

  private:
    std::unordered_set<ChunkPosition, ChunkPositionHash> m_sentChunks;

    Connection m_clientConnection;
    u32 m_salt = 0;

    u32 m_playerId = 0;

    bool m_isActive = false;

    int dist = 4;
};
