#pragma once

#include <SFML/System/Time.hpp>
#include <string>

#include <common/network/net_constants.h>

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
    bool shouldShowInstructions = true;

    std::string skinName = "player";
    std::string texturePack = "default";

    std::string serverIp = LOCAL_HOST;
};
