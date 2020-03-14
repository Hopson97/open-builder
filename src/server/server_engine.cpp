#include "server_engine.h"
#include "network/server.h"
#include "server_config.h"
#include <SFML/System/Clock.hpp>

#include <atomic>
#include <iostream>
#include <thread>

ServerLauncher::ServerLauncher(const ServerConfig& config, sf::Time timeout)
    : m_server(config)
    , m_config(config)
    , m_timeout(timeout)
{
}

void ServerLauncher::runServerEngine()
{
    if (!m_server.createAsServer(m_config.maxConnections)) {
        std::cout << "Failed to create server.\n";
        return;
    }
    bool serverRunning = true;
    sf::Clock clock;
    while (serverRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        // Server updates
        m_server.tick();
        m_server.update();

        // Exit the server if there is no connections
        if (m_server.getConnectedPeerCount() == 0) {
            serverRunning = clock.getElapsedTime() < m_timeout;
        }
        else {
            clock.restart();
        }
    }
    m_server.disconnectAllPeers();
    m_server.destroy();

    std::cout << "Server has now exited\n";
}