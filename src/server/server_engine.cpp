#include "server_engine.h"
#include "network/server.h"

void runServerEngine(const ServerConfig &config, sf::Time timeout)
{
    Server engine;
    engine.start(config, timeout);
}
