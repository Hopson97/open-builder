#pragma once

#include "../maths.h"
#include <common/network/net_host.h>
#include <common/world/chunk.h>

struct ClientState;

class Client : public NetworkHost {
  public:
    Client(ClientState &state);

    std::optional<peer_id_t> connectTo(const std::string &ipAddress);

    void sendDisconnectRequest();
    void sendPlayerPosition(const glm::vec3 &position);
    void sendChunkRequest(const ChunkPosition &position);

  private:
    void onPeerConnect(ENetPeer *peer) override;
    void onPeerDisconnect(ENetPeer *peer) override;
    void onPeerTimeout(ENetPeer *peer) override;
    void onCommandRecieve(ENetPeer *peer, sf::Packet &packet,
                          command_t command) override;

    void onPlayerJoin(sf::Packet &packet);
    void onPlayerLeave(sf::Packet &packet);
    void onSnapshot(sf::Packet &packet);
    void onChunkData(sf::Packet &packet);

    ClientState *const mp_clientState;
    ENetPeer *mp_serverPeer = nullptr;
};
