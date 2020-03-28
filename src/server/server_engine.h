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

    /**
     * @brief Construct a new Server Launcher object
     * @param timeout Time to wait before server exit after no connections are
     * connected
     */
    ServerLauncher(sf::Time timeout);
    ~ServerLauncher();

    void run();
    void runAsThread();
    void stop();

  private:
    void launch();

    Server m_server;
    std::unique_ptr<std::thread> m_serverThread;
    std::atomic_bool m_isServerRunning;

    sf::Time m_timeout;
};
