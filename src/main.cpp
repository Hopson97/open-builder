#include <common/debug.h>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "client/client_engine.h"
#include "server/server_engine.h"

#include "client/client_config.h"

#include <common/network/enet.h>
#include <common/util.h>

#include "client/window.h"

// Enable nvidia
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
extern "C" {
_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif

namespace {
    enum class LaunchType {
        Client, Server,
    };
    /**
     * @brief Loads config eg window size from the config.txt file
     * @param config The config object to put the data into
     */
    void loadFromConfigFile()
    {
        auto data = getObdData("config.obd");
        auto& config = ClientConfig::get();

        config.fullScreen = std::stoi(data["fullscreen"]);
        config.windowWidth = std::stoi(data["window_width"]);
        config.windowHeight = std::stoi(data["window_height"]);
        config.isFpsCapped = std::stoi(data["cap_fps"]);
        config.shouldShowInstructions = std::stoi(data["shouldShowInstructions"]);
        config.fpsLimit = std::stoi(data["fps_limit"]);
        config.fov = std::stoi(data["fov"]);
        config.renderDistance = std::stoi(data["renderDistance"]);
        config.fpsLimit = std::stoi(data["fps_limit"]);
        config.verticalSensitivity = std::stof(data["vertical_sensitivity"]);
        config.horizontalSensitivity = std::stof(data["horizontal_sensitivity"]);
        config.skinName = data["skin"];
        config.texturePack = data["texture_pack"];
    }

    LaunchType parseArgs(const std::vector<std::pair<std::string, std::string>>& args)
    {
        for (const auto& option : args) {
            if (option.first == "server") {
                return LaunchType::Server;
            }
        }
        return LaunchType::Client;
    }
} // namespace

int main(int argc, char** argv)
{
    if (enet_initialize() != 0) {
        return EXIT_FAILURE;
    }

    std::vector<std::pair<std::string, std::string>> args;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argc > i + 1) {
            args.emplace_back(argv[i], argv[i + 1]);
        }
    }

    loadFromConfigFile();
    switch (parseArgs(args)) {
        case LaunchType::Server: {
            ServerEngine launcher;
            launcher.run();
            break;
        }

        case LaunchType::Client: {
            sf::Window window;
            if (!Window::createWindowInitOpengl(window)) {
                return -1;
            }
            ClientEngine client;
            if (client.init(window)) {
                client.runClient();
            }
            break;
        }
    }

    enet_deinitialize();
}
