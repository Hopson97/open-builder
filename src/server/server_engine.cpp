#include "server_engine.h"
#include "network/server.h"
#include "server_config.h"
#include <SFML/System/Clock.hpp>

#include <thread>
#include <atomic>
#include <iostream>

void runServerEngine(const ServerConfig &config, sf::Time timeout)
{
    Server engine;
    engine.createAsServer(config.maxConnections);

    std::atomic<bool> serverRunning = true;
    std::atomic<bool> serverConsoleRunning = true;

    std::thread console([&serverRunning, &serverConsoleRunning]() {
        std::string line;
        while (serverRunning) {
            std::cout << "Type 'exit' to exit: ";
            std::getline(std::cin, line);
            if (line == "exit") {
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
        engine.tick();
        engine.sendPackets();

        // Exit the server if there is no connections
        if (engine.getConnectedPeerCount() == 0) {
            serverRunning = clock.getElapsedTime() < timeout;
        }
        else {
            clock.restart();
        }
    }
    engine.disconnectAllPeers();
    engine.destroy();
    if (serverConsoleRunning) {
        std::cout
            << "Server console is still active.\nPlease type anything to exit."
            << std::endl;
    }
    console.join();
}
