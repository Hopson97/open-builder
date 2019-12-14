#pragma once

#include <SFML/System/Time.hpp>
#include <array>
#include <common/network/net_host.h>
#include <unordered_map>

struct ServerConfig;

struct ServerEntity {
    float x = 0, y = 0, z = 0;
    bool active = false;
};

class Server : public NetworkHost {
  public:
    Server();

    void sendPackets();

  private:
    void onPeerConnect(ENetPeer &peer) override;
    void onPeerDisconnect(ENetPeer &peer) override;
    void onPeerTimeout(ENetPeer &peer) override;
    void onCommandRecieve(sf::Packet &packet, command_t command) override;

    void handleCommandDisconnect(sf::Packet &packet);
    void handleCommandPlayerPosition(sf::Packet &packet);

    int emptySlot() const;

    void addPeer(u32 connectionId, peer_id_t id);
    void removePeer(u32 connectionId);

    std::array<ServerEntity, 512> m_entities;
    std::array<bool, MAX_CONNECTIONS> m_peerConnected;
    std::unordered_map<u32, peer_id_t> m_peerIds;

    bool m_isRunning = true;
};