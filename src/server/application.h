#pragma once

#include <SFML/System/Clock.hpp>
#include <common/types.h>
class LaunchConfig;

namespace server {

    void runServerApp(const LaunchConfig &config, port_t port,
                      sf::Time timeout);
} // namespace server
