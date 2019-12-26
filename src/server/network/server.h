#pragma once

#include <SFML/System/Time.hpp>
#include <array>
#include <common/network/net_host.h>
#include <common/world/chunk_manager.h>
#include <queue>
#include <unordered_map>

struct ServerConfig;

struct ServerEntity {
    float x = 0, y = 0, z = 0;
    bool active = false;
};

struct ConnectedClient {
    ENetPeer *peer = nullptr;
    peer_id_t entityId;
    bool connected = false;
};

struct ChunkRequest {
    ChunkRequest(ChunkPosition &chunkPosition, peer_id_t peerId)
        : position(chunkPosition)
        , peer(peerId)
    {
    }

    ChunkPosition position;
    peer_id_t peer;
};

class Server final : public NetworkHost {
  public:
    Server();

    void sendPackets();

  private:
    void sendChunk(peer_id_t peerId, const Chunk &chunk);

    void onPeerConnect(ENetPeer *peer) override;
    void onPeerDisconnect(ENetPeer *peer) override;
    void onPeerTimeout(ENetPeer *peer) override;
    void onCommandRecieve(ENetPeer *peer, sf::Packet &packet,
                          command_t command) override;

    void handleCommandDisconnect(sf::Packet &packet);
    void handleCommandPlayerPosition(sf::Packet &packet);
    void handleCommandChunkRequest(sf::Packet &packet);

    int emptySlot() const;

    void addPeer(ENetPeer *peer, peer_id_t id);
    void removePeer(u32 connectionId);

    std::array<ServerEntity, 512> m_entities;
    std::array<ConnectedClient, MAX_CONNECTIONS> m_connectedClients{};

    ChunkManager m_chunkManager;
    Chunk *m_spawn;

    std::queue<ChunkRequest> m_chunkRequests;

    bool m_isRunning = true;
};