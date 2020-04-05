#pragma once

#include <common/network/enet.h>
#include <common/network/packet.h>

class ServerWorld;

struct PendingClientSession {
    Connection connection;
    u32 salt = 0;

    void sendHandshakeChallenge(u32 serversalt);
    void sendAcceptConnection(u32 playerId, ServerWorld& world);
    void sendRejectConnection(const char* reason);
};

class ClientSession {
  public:
    void init(ENetPeer* peer, u32 salt, u32 playerId);
    void disconnect();

    void sendPacket(const ServerPacket& packet, u32 channel = 0, u32 flags = 0);

    bool verify(u32 salt) const;
    bool isActive() const;
    u32 getPlayerId() const;

  private:
    Connection m_clientConnection;
    u32 m_salt = 0;

    u32 m_playerId = 0;

    bool m_isActive = false;
};