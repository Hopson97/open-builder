#include "server_engine.h"
#include "network/server.h"
#include <SFML/System/Clock.hpp>
#include <atomic>
#include <common/debug.h>
#include <iostream>
#include <thread>

ServerLauncher::ServerLauncher(sf::Time timeout)
    : m_server(16)
    , m_timeout(timeout)
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
                std::cout << "Exiting server.\n\n";
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
    if (!m_server.isSetup()) {
        return;
    }
    m_isServerRunning = true;
    LOG("Server ", "Launched.");
    while (m_isServerRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        m_server.tick();
    }
}

void ServerLauncher::stop()
{
    if (m_serverThread && m_serverThread->joinable()) {
        m_isServerRunning = false;
        if (m_serverThread->joinable()) {
            m_serverThread->join();
        }
        LOG("Server", "Exited.");
    }
}