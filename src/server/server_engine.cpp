#include "server_engine.h"
#include "network/server.h"
#include <SFML/System/Clock.hpp>
#include <atomic>
#include <common/debug.h>
#include <iostream>
#include <thread>

ServerLauncher::ServerLauncher(sf::Time timeout)
    : m_newServer(16)
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
    if (!m_newServer.isSetup()) {
        return;
    }
    m_isServerRunning = true;
    LOG("Server ", "Server has been launched.");
    while (m_isServerRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        m_newServer.tick();
    }
}

void ServerLauncher::stop()
{
    if (m_serverThread && m_serverThread->joinable()) {
        m_isServerRunning = false;
        if (m_serverThread->joinable()) {
            m_serverThread->join();
        }
        LOG("OLD_SERVER", "OLD_SERVER has exited.");
    }
}