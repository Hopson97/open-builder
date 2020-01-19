#pragma once

#include <SFML/System/Time.hpp>
#include <string>

/**
 * @brief Config options for client
 * Mostly for the window and general graphics options
 */
struct ClientConfig {
    bool fullScreen = false;
    int windowWidth = 1280;
    int windowHeight = 720;
    int fpsLimit = 60;
    int fov = 65;

    bool isFpsCapped = true;

    sf::Time connectionTimeout = sf::milliseconds(10000);

    std::string skinName = "player";
    std::string texturePack = "default";
};
