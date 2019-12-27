#include "server_engine.h"
#include "network/server.h"
#include "server_config.h"
#include <SFML/System/Clock.hpp>

#include <atomic>
#include <iostream>
#include <thread>

void runServerEngine(const ServerConfig &config, sf::Time timeout)
{
    Server engine;
    engine.createAsServer(config.maxConnections);

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
    if (serverConsoleRunning) {
        std::cout
            << "Server console is still active.\nPlease type anything to exit."
            << std::endl;
    }
    console.join();
}
