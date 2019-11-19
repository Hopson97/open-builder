#pragma once

#include <common/constants.h>
#include <vector>

#include "network/server.h"
#include "world/chunk/chunk.h"
#include "world/entity.h"

class LaunchConfig;

namespace server {

    void runServerApp(const LaunchConfig &config, port_t port,
                      sf::Time timeout);
} // namespace server
