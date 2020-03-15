#pragma once

#include "network/server.h"
#include "server_config.h"
#include <SFML/System/Time.hpp>
#include <atomic>
#include <thread>

class ServerLauncher {
  public:
    /**
     * @brief Construct a new Server Launcher object
     * @param config Config options of the server
     * @param timeout Time to wait before server exit after no connections are
     * connected
     */
    ServerLauncher(const ServerConfig& config, sf::Time timeout);

    void run();
    void stop();

  private:
    Server m_server;
    std::unique_ptr<std::thread> m_serverThread;
    std::atomic_bool m_isServerRunning;

    ServerConfig m_config;
    sf::Time m_timeout;
};
