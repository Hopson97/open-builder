#pragma once

#include <SFML/System/Time.hpp>
#include <string>

#include <common/network/net_constants.h>

/**
 * @brief Config options for client
 * Mostly for the window and general graphics options
 */
struct ClientConfig {
  public:
    static ClientConfig& get()
    {
        static ClientConfig config;
        return config;
    }

    ClientConfig& operator=(const ClientConfig&) = delete;
    ClientConfig& operator=(ClientConfig&&) = delete;

    ClientConfig(const ClientConfig&) = delete;
    ClientConfig(ClientConfig&&) = delete;

    bool fullScreen = false;
    int windowWidth = 1280;
    int windowHeight = 720;
    int fpsLimit = 60;
    int fov = 65;
    float renderDistance = 50.f;

    float verticalSensitivity = 1.f;
    float horizontalSensitivity = 1.f;

    bool isFpsCapped = true;
    bool shouldShowInstructions = true;

    std::string skinName = "player";
    std::string texturePack = "default";

  private:
    ClientConfig() = default;
};
