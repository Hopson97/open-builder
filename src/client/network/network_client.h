#pragma once

#include <common/macros.h>
#include <common/network/enet.h>
#include <common/network/net_command.h>

using ClientPacket = Packet<ClientCommand>;

enum class ConnectionState {
    Pending,
    Connected,
    Disconnected,
};

class NetworkClient final {
  public:
    NetworkClient();
    ~NetworkClient();

    ClientConnectionResult connectTo(const std::string& ipaddress);

    void tick();
    void disconnect();

    ConnectionState getConnnectionState() const;

  private:
    void handlePendingPacket(ClientPacket& packet);
    void handlePacket(ClientPacket& packet);

    void onHandshakeChallenge(ClientPacket& packet);

    ConnectionState m_connectionState = ConnectionState::Disconnected;
    Connection m_serverConnection;
    ENetHost* mp_host;

    u32 m_salt;

  public:
    NON_COPYABLE(NetworkClient)
    NON_MOVEABLE(NetworkClient)
};