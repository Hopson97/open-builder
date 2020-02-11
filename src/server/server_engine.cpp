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
    if (!m_server.createAsServer(m_config.get_maxConnections())) {
        std::cout << "Failed to create server.\n";
        return;
    }
    std::atomic<bool> serverRunning = true;
    std::atomic<bool> serverConsoleRunning = true;

    std::thread console([&serverRunning, &serverConsoleRunning]() {
        std::string line;
        std::cout << "This is the server console. You can enter commands here.";
        std::cout << "Server console: Enter exit to shut down server.";
        while (serverRunning) {
            std::cout << "> ";
            std::getline(std::cin, line);

            if (std::cin.eof() || line == "exit") {
                serverRunning = false;
                serverConsoleRunning = false;
                return;
            }
        }
    });

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
    if (serverConsoleRunning) {
        std::cout << "Server console is still active.\nPlease type anything to exit."
                  << std::endl;
    }
    console.join();
}
