#pragma once

#include "../lua/server_lua_callback.h"
#include "client_session.h"
#include <SFML/System/Time.hpp>
#include <array>
#include <common/lua/script_engine.h>
#include <common/network/net_command.h>
#include <common/network/packet.h>
#include <common/world/biome.h>
#include <common/world/chunk_manager.h>
#include <common/world/voxel_data.h>
#include <glm/gtc/matrix_transform.hpp>

class ServerWorld;

class Server {
  public:
    Server(int maxConnection, ServerWorld& world);
    ~Server();

    bool isSetup() const;
    void tick();

    void broadcastEntityStates();

  private:
    void broadcastPacket(ServerPacket& packet, int channel = 0, int flags = 0);

    void handlePacket(ServerPacket& packet, ENetPeer* peer);
    void addPendingConnection(ENetPeer* peer);

    void onHandshakePartOne(ServerPacket& packet, ENetPeer* peer);
    void onHandshakeResponse(ServerPacket& packet, ENetPeer* peer);

    void onPlayerState(ServerPacket& packet, ENetPeer* peer);


    void broadcastPlayerJoin(u32 playerId);
    void broadcastPlayerLeave(u32 playerId);
    void broadcastServerShutdown();

    int createClientSession(ENetPeer* peer, u32 salt);
    void handleDisconnection(ENetPeer* peer);

    std::vector<PendingClientSession>::iterator findPendingSession(u32 peerId);

    NetHost m_host;
    std::vector<ClientSession> m_clients;
    std::unordered_map<u32, int> m_clientsMap;

    std::vector<PendingClientSession> m_pendingConnections;

    int m_maxConnections = 0;

    u32 m_salt;

    ServerWorld* mp_world;
};