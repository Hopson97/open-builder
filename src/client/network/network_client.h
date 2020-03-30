#pragma once

#include <common/network/enet.h>

struct ConnectionResult {
    ConnectionResult() = default;
    ConnectionResult(const char* message);

    const char* message = nullptr;
    bool success = true;

    const static ConnectionResult SUCCESS;
};

enum class ConnectionState {
    Pending,
    Connected,
    Disconnected,
};

class NetworkClient {
  public:
    ConnectionResult connectTo(const std::string& ipaddress);

    void tick();

  private:
    ConnectionState m_connectionState = ConnectionState::Disconnected;
    Connection m_serverConnection;
    ENetHost* mp_host;
};