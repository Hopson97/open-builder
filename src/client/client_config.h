#pragma once

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

    std::string skinName = "player";
};
