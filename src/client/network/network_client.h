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

class NetworkClient final {
  public:
    NetworkClient();
    ~NetworkClient();
    NetworkClient(const NetworkClient&) = delete;
    NetworkClient(NetworkClient&&) = delete;
    NetworkClient& operator=(const NetworkClient&) = delete;
    NetworkClient& operator=(NetworkClient&& other) = delete;

    ConnectionResult connectTo(const std::string& ipaddress);

    void tick();
    void disconnect();

    ConnectionState getConnnectionState() const;

  private:
    ConnectionState m_connectionState = ConnectionState::Disconnected;
    Connection m_serverConnection;
    ENetHost* mp_host;
};