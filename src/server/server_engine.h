#pragma once

#include "network/client_session.h"
#include "world/server_world.h"
#include <SFML/System/Time.hpp>
#include <atomic>
#include <common/macros.h>
#include <thread>

class ServerEngine {
  public:
    NON_COPYABLE(ServerEngine)

    ServerEngine();
    ~ServerEngine();

    void run();
    void runAsThread();
    void stop();

    bool isSetup() const;

  private:
    void tick();

    void launch();

    void broadcastEntityStates();
    void broadcastPacket(ServerPacket& packet, int channel = 0, int flags = 0);

    void handlePacket(ServerPacket& packet, ENetPeer* peer);
    void addPendingConnection(ENetPeer* peer);

    void onHandshakePartOne(ServerPacket& packet, ENetPeer* peer);
    void onHandshakeResponse(ServerPacket& packet, ENetPeer* peer);

    void onInteraction(ServerPacket& packet, ENetPeer* peer);
    void onMouseState(ServerPacket& packet, ENetPeer* peer);
    void onPlayerState(ServerPacket& packet, ENetPeer* peer);
    void onSpawnRequest(ServerPacket& packet, ENetPeer* peer);

    void broadcastPlayerJoin(u32 playerId);
    void broadcastPlayerLeave(u32 playerId);
    void broadcastServerShutdown();

    int createClientSession(ENetPeer* peer, u32 salt);
    void handleDisconnection(ENetPeer* peer);
    std::vector<PendingClientSession>::iterator findPendingSession(u32 peerId);

    ServerWorld m_world;
    std::thread m_serverThread;
    std::atomic_bool m_isServerRunning;

    NetHost m_host;
    std::vector<ClientSession> m_clients;
    std::unordered_map<u32, int> m_clientsMap;

    std::vector<PendingClientSession> m_pendingConnections;

    int m_maxConnections = 0;

    u32 m_salt;
};
