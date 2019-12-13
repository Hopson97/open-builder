#pragma once

#include "enet.h"
#include "net_command.h"
#include "net_constants.h"
#include <SFML/Network/Packet.hpp>
#include <optional>

class NetworkHost {
  public:
    NetworkHost(std::string &&name);

    void tick();
    void destroy();
    void broadcast(sf::Packet &packet, u32 flags);

    std::optional<ENetPeer *> connectToServer(const std::string &ip);
    bool createAsServer();
    void disconnectFromPeer(ENetPeer &peer);

    int getConnectedPeerCount() const;
    peer_id_t getPeerId() const;

  protected:
    bool sendToPeer(ENetPeer &peer, sf::Packet &packet, u8 channel, u32 flags);
    void broadcastToPeers(sf::Packet &packet, u8 channel, u32 flags);

  private:
    virtual void onPeerConnect(ENetPeer &peer) = 0;
    virtual void onPeerDisconnect(ENetPeer &peer) = 0;
    virtual void onPeerTimeout(ENetPeer &peer) = 0;
    virtual void onCommandRecieve(sf::Packet &packet, command_t command) = 0;

    void onCommandRecieve(const ENetPacket &packet);

    void flush();

    ENetHost *mp_host = nullptr;
    const std::string m_name;

    peer_id_t m_peerId = 0;
};