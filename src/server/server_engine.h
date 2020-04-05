#pragma once

#include "network/server.h"
#include <SFML/System/Time.hpp>
#include <atomic>
#include <common/macros.h>
#include <thread>
#include "world/server_world.h"

class ServerEngine {
  public:
    NON_COPYABLE(ServerEngine)

    ServerEngine();
    ~ServerEngine();

    void run();
    void runAsThread();
    void stop();

  private:
    void launch();

    Server m_server;
    std::thread m_serverThread;
    std::atomic_bool m_isServerRunning;

    ServerWorld m_world;
};
