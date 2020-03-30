#pragma once

#include <common/network/enet.h>

struct ConnectionResult {
    ConnectionResult() = default;
    ConnectionResult(const char* message);

    const char* message;
    bool success = true;

    const static ConnectionResult SUCCESS;
};

class NetworkClient {
  public:
    ConnectionResult connectTo(const std::string& ipaddress);

    void tick();

  private:
    Connection m_serverConnection;
    ENetHost* mp_host;
};