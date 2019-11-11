#pragma once

#include <common/network/constants.h>

#include "game/entity.h"
#include "network/server.h"

class LaunchConfig;

namespace server {
    class Application {
      public:
        Application(const LaunchConfig &config, Port port);

        void run(sf::Time timeout);

      private:
        void update(sf::Time deltaTime);

        EntityArray m_entities;
        Server m_server;

        bool m_isRunning = true;
    };
} // namespace server
