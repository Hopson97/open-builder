#pragma once

#include <common/macros.h>
#include <common/network/enet.h>
#include <common/network/net_command.h>
#include <common/network/packet.h>

struct EntityState;
class ClientWorld;

enum class ConnectionState {
    Pending,
    Connected,
    Disconnected,
};

class Client final {
  public:
    Client();
    ~Client();

    void setWorld(ClientWorld& world);

    ClientConnectionResult connectTo(const std::string& ipaddress);

    void tick();
    void disconnect();

    ConnectionState getConnnectionState() const;

    void sendPlayerState(const EntityState& state);

  private:
    void handlePacket(ClientPacket& packet);

    void onHandshakeChallenge(ClientPacket& packet);
    void onConnectionAcceptance(ClientPacket& packet);

    void onAddEntity(ClientPacket& packet);
    void onRemoveEntity(ClientPacket& packet);
    void onForceExit(ClientPacket& packet);

    ConnectionState m_connectionState = ConnectionState::Disconnected;
    Connection m_serverConnection;
    NetHost m_host;

    ClientWorld* mp_world = nullptr;

    u32 m_salt;

  public:
    NON_COPYABLE(Client)
    NON_MOVEABLE(Client)
};