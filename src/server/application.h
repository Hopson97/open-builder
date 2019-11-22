#pragma once

#include <SFML/System/Clock.hpp>
#include <common/types.h>

namespace server {
    struct Config;

    void runServerApp(const Config &config, port_t port,
                      sf::Time timeout);
} // namespace server
