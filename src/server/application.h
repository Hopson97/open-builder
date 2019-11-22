#pragma once

#include <SFML/System/Time.hpp>

namespace server {
    struct Config;

    void runServerApp(const Config &config, sf::Time timeout);
} // namespace server
