#pragma once

#include <SFML/System/Time.hpp>
#include <array>
#include <common/network/net_host.h>

struct ServerConfig;

struct ServerEntity {
    float x = 0, y = 0, z = 0;
    bool active = false;
};

class Server : public NetworkHost {
  public:
    Server();

    void start(const ServerConfig &config, sf::Time timeout);

  private:
    void onPeerConnect(ENetPeer &peer) override;
    void onPeerDisconnect(ENetPeer &peer) override;
    void onPeerTimeout(ENetPeer &peer) override;
    void onCommandRecieve(sf::Packet &packet, command_t command) override;

    void handleCommandDisconnect(sf::Packet &packet);
    void handleCommandPlayerPosition(sf::Packet &packet);

    void sendPackets();

    int emptySlot() const;

    std::array<ServerEntity, 512> m_entities;
    std::array<bool, MAX_CONNECTIONS> m_peerConnected;

    bool m_isRunning = true;
};