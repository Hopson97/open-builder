#include "server_engine.h"
#include "network/server.h"
#include <SFML/System/Clock.hpp>
#include <atomic>
#include <common/debug.h>
#include <iostream>
#include <thread>

ServerLauncher::ServerLauncher()
    : m_server(16)
{
}

ServerLauncher::~ServerLauncher()
{
    stop();
}

void ServerLauncher::run()
{
    printf("Type 'exit' to shutdown server");
    m_serverThread = std::thread([this] {
        std::string input;
        while (m_isServerRunning) {
            std::getline(std::cin, input);
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
    m_serverThread = std::thread([this] { launch(); });
}

void ServerLauncher::launch()
{
    if (!m_server.isSetup()) {
        return;
    }
    m_isServerRunning = true;
    while (m_isServerRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        m_server.tick();
    }
}

void ServerLauncher::stop()
{
    m_isServerRunning = false;
    if (m_serverThread.joinable()) {
        m_serverThread.join();
    }
}