#pragma once

#include <SFML/System/Time.hpp>
#include <array>
#include <common/network/net_host.h>
#include <common/world/chunk.h>
#include <queue>
#include <unordered_map>

struct ServerConfig;

struct ServerEntity {
    float x = 0, y = 0, z = 0;
    bool active = false;
};

struct Peer {
    ENetPeer *peer;
    peer_id_t peerId;
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
    ENetPeer *findPeer(peer_id_t peerId);

    void sendChunk(peer_id_t peerId, const Chunk &chunk);

    void onPeerConnect(ENetPeer &peer) override;
    void onPeerDisconnect(ENetPeer &peer) override;
    void onPeerTimeout(ENetPeer &peer) override;
    void onCommandRecieve(sf::Packet &packet, command_t command) override;

    void handleCommandDisconnect(sf::Packet &packet);
    void handleCommandPlayerPosition(sf::Packet &packet);
    void handleCommandChunkRequest(sf::Packet &packet);

    int emptySlot() const;

    void addPeer(ENetPeer *peer, peer_id_t id);
    void removePeer(u32 connectionId);

    std::array<ServerEntity, 512> m_entities;
    std::array<bool, MAX_CONNECTIONS> m_peerConnected{false};
    std::unordered_map<u32, Peer> m_peers;

    ChunkManager m_chunkManager;
    Chunk *m_spawn;

    std::queue<ChunkRequest> m_chunkRequests;

    bool m_isRunning = true;
};