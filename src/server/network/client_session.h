#pragma once

#include <common/network/enet.h>

struct PendingSession {
  public:
    PendingSession(ENetPeer* peer);

    void sendChallenge(int);

  private:
    Connection m_connection;
};

class ClientSession {
  public:
    void init();
    void disconnect();

  private:
    Connection m_clientConnection;
};