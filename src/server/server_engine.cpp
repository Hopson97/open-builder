#include "server_engine.h"
#include "network/server.h"
#include <SFML/System/Clock.hpp>
#include <atomic>
#include <common/debug.h>
#include <iostream>
#include <thread>

ServerEngine::ServerEngine()
    : m_server(16, m_world)
{
}

ServerEngine::~ServerEngine()
{
    stop();
}

void ServerEngine::run()
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

void ServerEngine::runAsThread()
{
    m_serverThread = std::thread([this] { launch(); });
}

void ServerEngine::launch()
{
    if (!m_server.isSetup()) {
        return;
    }
    m_isServerRunning = true;
    while (m_isServerRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        m_server.tick();
        m_world.tick();
        
        m_server.broadcastEntityStates();
    }
}

void ServerEngine::stop()
{
    m_isServerRunning = false;
    if (m_serverThread.joinable()) {
        m_serverThread.join();
    }
}