#pragma once

#include "network/server.h"
#include "world/entity.h"
#include <common/constants.h>
#include <common/world/chunk.h>
#include <vector>

class LaunchConfig;

namespace server {
    class Application {
      public:
        Application(const LaunchConfig &config, port_t port);

        void run(sf::Time timeout);

      private:
        void update(sf::Time deltaTime);
        void sendState();

        EntityArray m_entities;
        Server m_server;

        bool m_isRunning = true;

        std::vector<Chunk> m_chunks;
    };
} // namespace server
