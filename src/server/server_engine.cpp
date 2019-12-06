#include "server_engine.h"

#include "network/server.h"
#include "server_config.h"
#include <SFML/System/Clock.hpp>
#include <array>
#include <common/debug.h>
#include <common/network/net_command.h>
#include <common/network/packet.h>
#include <ctime>
#include <iostream>
#include <random>
#include <thread>

namespace {
    
} // namespace

void runServerEngine(const ServerConfig &config, sf::Time timeout)
{
    // Start the main server loop
    sf::Clock deltaClock;
    sf::Clock timeoutClock;

    Server server;

    bool isRunning = true;
    while (isRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        auto deltaTime = deltaClock.restart();

		server.recievePackets();
        server.tick();

		//==============
		//	Check if server should auto shutdown
		//
        if (server.clients().connectedCount() == 0) {
            if (timeoutClock.getElapsedTime() >= timeout) {
                isRunning = false;
            }
        }
        else {
            timeoutClock.restart();
        }
        (void)deltaTime;
        (void)config;
    }
    LOG("Server exiting");
}
