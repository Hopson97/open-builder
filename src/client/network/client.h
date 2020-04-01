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

class Client final {
  public:
    Client();
    ~Client();

    ClientConnectionResult connectTo(const std::string& ipaddress);

    void tick();
    void disconnect();

    ConnectionState getConnnectionState() const;

  private:
    void handlePacket(ClientPacket& packet);

    void onHandshakeChallenge(ClientPacket& packet);
    void onConnectionAcceptance(ClientPacket& packet);

    ConnectionState m_connectionState = ConnectionState::Disconnected;
    Connection m_serverConnection;
    NetHost m_host;

    u32 m_salt;

  public:
    NON_COPYABLE(Client)
    NON_MOVEABLE(Client)
};