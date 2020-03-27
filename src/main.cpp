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
#include "server/server_config.h"

#include <common/network/enet.h>
#include <common/util/obd_parser.h>

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
        Server,
        Client,
    };

    /**
     * @brief Holds config for both client and server
     */
    struct Config {
        LaunchType launchType = LaunchType::Client;

        ServerConfig server;
        ClientConfig client;
    };

    /**
     * @brief Loads config eg window size from the config.txt file
     * @param config The config object to put the data into
     */
    void loadFromConfigFile(Config& config)
    {
        auto data = getObdData("config.obd");
        auto clientData = data[0].data;
        auto serverData = data[1].data;

        config.client.fullScreen = std::stoi(clientData["fullscreen"]);
        config.client.windowWidth = std::stoi(clientData["window_width"]);
        config.client.windowHeight = std::stoi(clientData["window_height"]);
        config.client.isFpsCapped = std::stoi(clientData["cap_fps"]);
        config.client.shouldShowInstructions =
            std::stoi(clientData["shouldShowInstructions"]);
        config.client.fpsLimit = std::stoi(clientData["fps_limit"]);
        config.client.fov = std::stoi(clientData["fov"]);
        config.client.renderDistance = std::stoi(clientData["renderDistance"]);
        config.client.fpsLimit = std::stoi(clientData["fps_limit"]);
        config.client.verticalSensitivity = std::stof(clientData["vertical_sensitivity"]);
        config.client.horizontalSensitivity =
            std::stof(clientData["horizontal_sensitivity"]);
        config.client.skinName = clientData["skin"];
        config.client.texturePack = clientData["texture_pack"];
        config.client.serverIp = clientData["server_ip"];
        if (config.client.serverIp == "LOCAL") {
            config.client.serverIp = LOCAL_HOST;
        }

        config.server.worldSize = std::stoi(serverData["world_size"]);
    }

    /**
     * @brief Parses the CLI arguments from the user
     * @param config The config to load data into
     * @param args CLI arguments paired as <argument, param>
     */
    void parseArgs(Config& config,
                   const std::vector<std::pair<std::string, std::string>>& args)
    {
        for (const auto& option : args) {
            // Set launch type to be server.
            // Option: MAX_CONNECTIONS 2-16
            if (option.first == "-server") {
                config.launchType = LaunchType::Server;
                try {
                    int maxConnections = std::stoi(option.second);
                    if (maxConnections < 2) {
                        throw std::invalid_argument("Max connections must be at least " +
                                                    std::to_string(MIN_CONNECTIONS) +
                                                    ".\n");
                    }
                    else if (maxConnections > 16) {
                        throw std::invalid_argument("Max connections must be " +
                                                    std::to_string(MAX_CONNECTIONS) +
                                                    " or below.\n");
                    }
                }
                catch (std::invalid_argument& e) {
                    std::cout << "Unable to set max connections, defaulting to "
                                 "4. Reason: "
                              << e.what() << "\n";
                    config.server.maxConnections = 4;
                }
            }
            else if (option.first == "-client") {
                config.launchType = LaunchType::Client;
            }
            else if (option.first == "-skin") {
                config.client.skinName = option.second;
            }
        }
    }

    /**
     * @brief Prints failure message
     * @return int Exit failure flag
     */
    int exitFailure(const char* message)
    {
        std::cerr << "Engine exited with error: \"" << message << "\"." << '\n';
        return EXIT_FAILURE;
    }

    void printInstructions()
    {
        const int width = 20;
        auto printInstruction = [width](const char* input, const char* output) {
            std::cout << std::setw(width) << std::left << output << input << '\n';
        };
        std::cout << "Take a look at the instructions before you play." << '\n'
                  << "And also remember that the default configurations are on "
                     "the config.obd file"
                  << '\n';

        std::cout << std::setw(width) << std::left << "Action"
                  << "Key/Mouse" << '\n'
                  << std::setw(width + 10) << std::setfill('-') << "" << '\n'
                  << std::setfill(' ') << std::setw(width) << std::left;
        printInstruction("W", "Move Forwards");
        printInstruction("A", "Move Left");
        printInstruction("S", "Move Back");
        printInstruction("D", "Move Right");

        printInstruction("CTRL", "Sprint");
        printInstruction("Right Click", "Place A Voxel");
        printInstruction("Left Click", "Removes A Voxel");

        printInstruction("Move Mouse", "Look");

        printInstruction("ESC", "Exit Game");

        std::cout << "Press Enter to Continue...";
        std::cin.ignore();
    }
} // namespace

int main(int argc, char** argv)
{

    Config config;

    if (enet_initialize() != 0) {
        return exitFailure("Failed to initialise enet");
    }

    std::vector<std::pair<std::string, std::string>> args;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argc > i + 1) {
            args.emplace_back(argv[i], argv[i + 1]);
        }
    }

    loadFromConfigFile(config);
    parseArgs(config, args);

    // return launchClient(config.client, false);

    switch (config.launchType) {
        case LaunchType::Server: {
            ServerLauncher launcher(config.server, sf::seconds(0));
            launcher.run();
            break;
        }

        case LaunchType::Client: {
            runClientEngine(config.client);
            break;
        }
    }

    enet_deinitialize();
}
