#pragma once

#include <SFML/System/Time.hpp>

struct ServerConfig;

void runServerEngine(const ServerConfig &config, sf::Time timeout);
