#pragma once

#include <common/network/constants.h>

#include "world/entity.h"
#include "network/server.h"

class LaunchConfig;

namespace server {
    class Application {
      public:
        Application(const LaunchConfig &config, Port port);

        void run(sf::Time timeout);

      private:
        void update(sf::Time deltaTime);
        void sendState();

        EntityArray m_entities;
        Server m_server;

        bool m_isRunning = true;
    };
} // namespace server
