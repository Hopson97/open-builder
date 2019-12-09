#pragma once

#include <SFML/System/Time.hpp>

struct ServerConfig;

/**
 * @brief Run the server engine
 *
 * @param config Config options of the server
 * @param timeout Time to wait before server exit after no connections are
 * connected
 */
void runServerEngine(const ServerConfig &config, sf::Time timeout);
