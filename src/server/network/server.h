#pragma once

#include "../world/server_voxel.h"
#include <SFML/System/Time.hpp>
#include <array>
#include <common/network/net_host.h>
#include <common/world/chunk_manager.h>
#include <common/world/voxel_registry.h>
#include <glm/gtc/matrix_transform.hpp>
#include <queue>
#include <unordered_map>

struct ServerConfig;

struct ServerEntity {
    glm::vec3 position{0.0f};
    bool active = false;

    std::vector<sf::Uint8> m_skinData;
    bool hasSkin = false;
};

struct ConnectedClient {
    ENetPeer *peer = nullptr;
    peer_id_t entityId = 0;
    bool connected = false;
};

class Server final : public NetworkHost {
    struct BlockUpdate {
        BlockPosition position;
        block_t block;
    };

  public:
    Server(const ServerConfig &config);

    void update();

  private:
    glm::vec3 findPlayerSpawnPosition();

    void sendChunk(peer_id_t peerId, const ChunkPosition &chunk);
    void sendPlayerSkin(peer_id_t peerId,
                        std::optional<peer_id_t> toPeer = std::nullopt);
    void sendGameData(peer_id_t peerId);

    void onPeerConnect(ENetPeer *peer) override;
    void onPeerDisconnect(ENetPeer *peer) override;
    void onPeerTimeout(ENetPeer *peer) override;
    void onCommandRecieve(ENetPeer *peer, sf::Packet &packet,
                          command_t command) override;

    void handleCommandPlayerPosition(sf::Packet &packet);
    void handleCommandBlockEdit(sf::Packet &packet);
    void handleCommandPlayerSkin(sf::Packet &packet);

    int findEmptySlot() const;

    void addPeer(ENetPeer *peer, peer_id_t id);
    void removePeer(u32 connectionId);

    std::array<ServerEntity, 512> m_entities;
    std::array<ConnectedClient, MAX_CONNECTIONS> m_connectedClients{};

    struct {
        ChunkManager chunks;
        std::vector<BlockUpdate> blockUpdates;
    } m_world;

    bool m_isRunning = true;
    const int m_worldSize;

    VoxelRegistry<ServerVoxel> m_voxelRegistry;
};