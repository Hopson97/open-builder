#pragma once

#include <common/network/enet.h>

class ClientSession {
  public:
    void init();
    void disconnect();

  private:
    Connection m_clientConnection;
};