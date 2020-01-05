#pragma once

#include <SFML/System/Time.hpp>
#include <array>
#include <common/network/net_host.h>
#include <common/world/chunk_manager.h>
#include <queue>
#include <unordered_map>

#include <glm/gtc/matrix_transform.hpp>

struct ServerConfig;

struct ServerEntity {
    glm::vec3 position{0.0f};
    bool active = false;
};

struct ConnectedClient {
    ENetPeer *peer = nullptr;
    peer_id_t entityId;
    bool connected = false;
};

class Server final : public NetworkHost {
  public:
    Server();

    void sendPackets();

  private:
    glm::vec3 findPlayerSpawnPosition();

    void sendChunk(peer_id_t peerId, const ChunkPosition &chunk);

    void onPeerConnect(ENetPeer *peer) override;
    void onPeerDisconnect(ENetPeer *peer) override;
    void onPeerTimeout(ENetPeer *peer) override;
    void onCommandRecieve(ENetPeer *peer, sf::Packet &packet,
                          command_t command) override;

    void handleCommandPlayerPosition(sf::Packet &packet);

    int findEmptySlot() const;

    void addPeer(ENetPeer *peer, peer_id_t id);
    void removePeer(u32 connectionId);

    std::array<ServerEntity, 512> m_entities;
    std::array<ConnectedClient, MAX_CONNECTIONS> m_connectedClients{};

    struct {
        ChunkManager chunks;
    } m_world;
    bool m_isRunning = true;
};