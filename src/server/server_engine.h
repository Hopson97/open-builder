#pragma once

#include "network/server.h"
#include <SFML/System/Time.hpp>
#include <atomic>
#include <common/macros.h>
#include <thread>

class ServerLauncher {
  public:
    NON_COPYABLE(ServerLauncher)
    NON_MOVEABLE(ServerLauncher)

    ServerLauncher(sf::Time timeout);
    ~ServerLauncher();

    void run();
    void runAsThread();
    void stop();

  private:
    void launch();

    Server m_server;
    std::thread m_serverThread;
    std::atomic_bool m_isServerRunning;

    sf::Time m_timeout;
};
