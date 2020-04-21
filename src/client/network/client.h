#pragma once

#include <common/macros.h>
#include <common/network/enet.h>
#include <common/network/net_command.h>
#include <common/network/packet.h>

struct EntityState;
class ClientWorld;
class Player;

enum class ConnectionState {
    Pending,
    Connected,
    Disconnected,
};

enum class MouseEventState { Click, Release };

class Client final {
  public:
    Client(ClientWorld& world, Player& player);
    ~Client();

    ClientConnectionResult connectTo(const std::string& ipaddress);

    void tick();
    void disconnect();

    ConnectionState getConnnectionState() const;

    void sendInteraction();
    void sendMouseEvent(MouseEventState state);
    void sendPlayerState(const EntityState& state);
    void sendSpawnRequest();

  private:
    void handlePacket(ClientPacket& packet);

    void onHandshakeChallenge(ClientPacket& packet);
    void onConnectionAcceptance(ClientPacket& packet);
    void onGameData(ClientPacket& packet);

    void onAddEntity(ClientPacket& packet);
    void onAddChunk(ClientPacket& packet);
    void onForceExit(ClientPacket& packet);
    void onPlayerSpawnPoint(ClientPacket& packet);
    void onRemoveEntity(ClientPacket& packet);
    void onUpdateEntityStates(ClientPacket& packet);
    void onVoxelUpdate(ClientPacket& packet);

    ConnectionState m_connectionState = ConnectionState::Disconnected;
    Connection m_serverConnection;
    NetHost m_host;

    ClientWorld* mp_world = nullptr;
    Player* mp_player = nullptr;

    u32 m_salt;

  public:
    NON_COPYABLE(Client)
    NON_MOVEABLE(Client)
};
