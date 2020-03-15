#include "server_engine.h"
#include "network/server.h"
#include "server_config.h"
#include <SFML/System/Clock.hpp>
#include <atomic>
#include <common/debug.h>
#include <iostream>
#include <thread>

ServerLauncher::ServerLauncher(const ServerConfig& config, sf::Time timeout)
    : m_server(config)
    , m_config(config)
    , m_timeout(timeout)
{
}

void ServerLauncher::run()
{
    if (!m_server.createAsServer(m_config.maxConnections)) {
        std::cout << "Failed to create server.\n";
        return;
    }

    m_serverThread = std::make_unique<std::thread>([this] {
        sf::Clock clock;
        m_isServerRunning = true;
        LOG("Server", "Launching server");
        while (m_isServerRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));

            // Server updates
            m_server.tick();
            m_server.update();

            // Exit the server if there is no connections
            if (m_server.getConnectedPeerCount() == 0) {
                m_isServerRunning = clock.getElapsedTime() < m_timeout;
            }
            else {
                clock.restart();
            }
        }
    });
}

void ServerLauncher::stop()
{
    if (m_isServerRunning) {
        m_isServerRunning = false;
        m_serverThread->join();

        m_server.disconnectAllPeers();
        m_server.destroy();
        LOG("Server", "Server has exited.");
    }
}