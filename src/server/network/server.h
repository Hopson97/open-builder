#pragma once

#include "../lua/server_lua_callback.h"
#include "client_session.h"
#include <SFML/System/Time.hpp>
#include <array>
#include <common/lua/script_engine.h>
#include <common/network/command_dispatcher.h>
#include <common/network/net_host.h>
#include <common/world/biome.h>
#include <common/world/chunk_manager.h>
#include <common/world/voxel_data.h>
#include <glm/gtc/matrix_transform.hpp>

using ServerPacket = Packet<ServerCommand>;

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

    ENetHost* mp_host = nullptr;
    std::vector<ClientSession> m_clients;
    std::unordered_map<u32, ClientSession*> m_clientsMap;

    std::vector<Connection> m_pendingConnections;

    int m_maxConnections = 0;

    u32 m_salt;
};

struct ServerConfig;

struct ServerEntity {
    glm::vec3 position{0.0f};
    bool active = false;

    std::vector<sf::Uint8> m_skinData;
    bool hasSkin = false;
};

struct ConnectedClient {
    ENetPeer* peer = nullptr;
    peer_id_t entityId = 0;
    bool connected = false;
};

class OLD_SERVER final : public NetworkHost {
    struct VoxelUpdate {
        VoxelPosition position;
        voxel_t voxel;
    };

  public:
    OLD_SERVER();

    void update();

  private:
    glm::vec3 findPlayerSpawnPosition();

    void sendChunk(peer_id_t peerId, const ChunkPosition& chunk);
    void sendPlayerSkin(peer_id_t peerId, std::optional<peer_id_t> toPeer = std::nullopt);
    void sendGameData(peer_id_t peerId);

    void onPeerConnect(ENetPeer* peer) override;
    void onPeerDisconnect(ENetPeer* peer) override;
    void onPeerTimeout(ENetPeer* peer) override;
    void onCommandRecieve(ENetPeer* peer, sf::Packet& packet, command_t command) override;

    void onPlayerPosition(sf::Packet& packet);
    void onVoxelEdit(sf::Packet& packet);
    void onPlayerSkin(sf::Packet& packet);

    int findEmptySlot() const;

    void addPeer(ENetPeer* peer, peer_id_t id);
    void removePeer(u32 connectionId);

    std::array<ServerEntity, 512> m_entities;
    std::array<ConnectedClient, MAX_CONNECTIONS> m_connectedClients{};

    struct {
        ChunkManager chunks;
        std::vector<VoxelUpdate> voxelUpdates;
    } m_world;

    bool m_isRunning = true;
    const int m_worldSize;

    ScriptEngine m_script;
    ServerLuaCallbacks m_luaCallbacks;
    VoxelDataManager m_voxelData;
    BiomeDataManager m_biomeData;

    CommandDispatcher<OLD_SERVER, ServerCommand> m_commandDispatcher;
};