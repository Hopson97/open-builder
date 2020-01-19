#pragma once

#include "network/server.h"
#include "server_config.h"
#include <SFML/System/Time.hpp>

class ServerLauncher {
  public:
    /**
     * @brief Construct a new Server Launcher object
     * @param config Config options of the server
     * @param timeout Time to wait before server exit after no connections are
     * connected
     */
    ServerLauncher(const ServerConfig &config, sf::Time timeout);

    /**
     * @brief Run the server engine
     */
    void runServerEngine();

  private:
    Server m_server;
    ServerConfig m_config;
    sf::Time m_timeout;
};
