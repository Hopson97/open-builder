#pragma once

#include <common/macros.h>
#include <common/network/enet.h>

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
    ConnectionState m_connectionState = ConnectionState::Disconnected;
    Connection m_serverConnection;
    ENetHost* mp_host;

  public:
    NON_COPYABLE(NetworkClient)
    NON_MOVEABLE(NetworkClient)
};