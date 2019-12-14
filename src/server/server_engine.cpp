#include "server_engine.h"
#include "network/server.h"
#include "server_config.h"
#include <SFML/System/Clock.hpp>

#include <thread>

void runServerEngine(const ServerConfig &config, sf::Time timeout)
{
    Server engine;
    engine.createAsServer(config.maxConnections);

    bool serverRunning = true;
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
    engine.destroy();
}
