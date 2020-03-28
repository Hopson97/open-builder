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

    /**
     * @brief Parses the CLI arguments from the user
     * @param config The config to load data into
     * @param args CLI arguments paired as <argument, param>
     */
    LaunchType parseArgs(const std::vector<std::pair<std::string, std::string>>& args)
    {

        LaunchType launchType = LaunchType::Client;
        for (const auto& option : args) {
            // Set launch type to be server.
            // Option: MAX_CONNECTIONS 2-16
            if (option.first == "-server") {
                launchType = LaunchType::Server;
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
                    // config.server.maxConnections = 4;
                }
            }
            else if (option.first == "-client") {
                launchType = LaunchType::Client;
            }
            else if (option.first == "-skin") {
                ClientConfig::get().skinName = option.second;
            }
        }
        return launchType;
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
    if (enet_initialize() != 0) {
        return exitFailure("Failed to initialise enet");
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
            ServerLauncher launcher(sf::seconds(0));
            launcher.run();
            break;
        }

        case LaunchType::Client: {
            runClientEngine();
            break;
        }
    }

    enet_deinitialize();
}
