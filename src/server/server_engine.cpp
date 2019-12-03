#include "server_engine.h"

#include "server_config.h"
#include <SFML/System/Clock.hpp>
#include <iostream>
#include <thread>


void runServerEngine(const ServerConfig &config, sf::Time timeout)
{
    // Start the main server loop
    sf::Clock deltaClock;
    while (0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        auto deltaTime = deltaClock.restart();

        (void)deltaTime;
        (void)config;
        (void)timeout;
    }
}

