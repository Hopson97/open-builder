#include "server_engine.h"
#include "network/server.h"
#include <SFML/System/Clock.hpp>
#include <atomic>
#include <common/debug.h>
#include <iostream>
#include <thread>

ServerLauncher::ServerLauncher(sf::Time timeout)
    : m_timeout(timeout)
{
}

ServerLauncher::~ServerLauncher()
{
    stop();
}

void ServerLauncher::run()
{

    std::cout << "Server Console Commands:\n"
              << "exit - Exits server and disconnects everyone\n\n";

    m_serverThread = std::make_unique<std::thread>([this] {
        std::string input;
        while (m_isServerRunning) {
            std::cin >> input;

            if (input == "exit") {
                std::cout << "Exiting Server.\n\n";
                m_isServerRunning = false;
            }
        }
    });

    launch();
}

void ServerLauncher::runAsThread()
{
    m_serverThread = std::make_unique<std::thread>([this] { launch(); });
}

void ServerLauncher::launch()
{
    if (!m_server.createAsServer(16)) {
        std::cout << "Failed to create server.\n\n";
        return;
    }
    sf::Clock clock;
    m_isServerRunning = true;
    LOG("Server", "Server has been launched.");
    while (m_isServerRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        // Server updates
        m_server.tick();
        m_server.update();

        // Exit the server if there is no connections
        if (m_timeout > sf::seconds(0)) {
            if (m_server.getConnectedPeerCount() == 0) {
                m_isServerRunning = clock.getElapsedTime() < m_timeout;
            }
            else {
                clock.restart();
            }
        }
    }
    LOG("Server", "Server stopped.");
}

void ServerLauncher::stop()
{
    if (m_serverThread && m_serverThread->joinable()) {
        m_isServerRunning = false;
        if (m_serverThread->joinable()) {
            m_serverThread->join();
        }

        m_server.disconnectAllPeers();
        m_server.destroy();
        LOG("Server", "Server has exited.");
    }
}