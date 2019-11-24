#include "server_engine.h"

#include "server_config.h"
#include <iostream>
#include <thread>
#include <SFML/System/Clock.hpp>

namespace server {
    void runServerEngine(const Config &config, sf::Time timeout)
    {
        // Start the main server loop
        sf::Clock deltaClock;
        while (0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            auto deltaTime = deltaClock.restart();
        }
    }
} // namespace server
