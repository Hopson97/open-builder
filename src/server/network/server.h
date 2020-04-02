#pragma once

#include "../lua/server_lua_callback.h"
#include "client_session.h"
#include <SFML/System/Time.hpp>
#include <array>
#include <common/lua/script_engine.h>
#include <common/world/biome.h>
#include <common/world/chunk_manager.h>
#include <common/world/voxel_data.h>
#include <glm/gtc/matrix_transform.hpp>
#include <common/network/net_command.h>
#include <common/network/packet.h>

using ServerPacket = Packet<ServerCommand, ClientCommand>;

class Server {
  public:
    Server(int maxConnection);
    ~Server();

    bool isSetup() const;
    void tick();

  private:
    void handlePacket(ServerPacket& packet, ENetPeer* peer);
    void addPendingConnection(ENetPeer* peer);

    void onHandshakePartOne(ServerPacket& packet, ENetPeer* peer);
    void onHandshakeResponse(ServerPacket& packet, ENetPeer* peer);

    int createClientSession(ENetPeer* peer, u32 salt);

    NetHost m_host;
    std::vector<ClientSession> m_clients;
    std::unordered_map<u32, ClientSession*> m_clientsMap;

    std::vector<PendingClientSession> m_pendingConnections;

    int m_maxConnections = 0;

    u32 m_salt;
};