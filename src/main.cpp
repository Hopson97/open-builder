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
#include <common/obd_parser.h>

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
    Both,
    TwoPlayer,
};

/**
 * @brief Holds config for both client and server
 */
struct Config {
    LaunchType launchType = LaunchType::Both;

    ServerConfig server;
    ClientConfig client;
};

/**
 * @brief Loads config eg window size from the config.txt file
 * @param config The config object to put the data into
 */
void loadFromConfigFile(Config &config)
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
    config.client.fpsLimit = std::stoi(clientData["fps_limit"]);
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
void parseArgs(Config &config,
               const std::vector<std::pair<std::string, std::string>> &args)
{
    for (const auto &option : args) {
        // Set launch type to be server.
        // Option: MAX_CONNECTIONS 2-16
        if (option.first == "-server") {
            config.launchType = LaunchType::Server;
            try {
                int maxConnections = std::stoi(option.second);
                if (maxConnections < 2) {
                    throw std::invalid_argument(
                        "Max connections must be at least " +
                        std::to_string(MIN_CONNECTIONS) + ".\n");
                }
                else if (maxConnections > 16) {
                    throw std::invalid_argument(
                        "Max connections must be " +
                        std::to_string(MAX_CONNECTIONS) + " or below.\n");
                }
            }
            catch (std::invalid_argument &e) {
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
 * @brief Prints success message
 * @return int Exit success flag
 */
int exitSuccess(const char *message = "Normal exit")
{
    std::cout << "Engine exited successfully.\"" << message << "\"." << '\n';
    return EXIT_SUCCESS;
}

/**
 * @brief Prints failure message
 * @return int Exit failure flag
 */
int exitFailure(const char *message)
{
    std::cerr << "Engine exited with error: \"" << message << "\"." << '\n';
    return EXIT_FAILURE;
}

/**
 * @brief Launches the server
 * @param config Config to be used by the server engine
 * @param timeout How long the server waits for a connection before closing
 * @return int Exit success flag
 */
int launchServer(const ServerConfig &config, sf::Time timeout = sf::seconds(8))
{
    LOG("Launcher", "Launching server");
    ServerLauncher launcher(config, timeout);
    launcher.runServerEngine();
    LOG("Launcher", "Server has exited.");
    return EXIT_SUCCESS;
}

void printInstructions()
{
    const int width = 20;
    auto printInstruction = [width](const char *input, const char *output) {
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
    printInstruction("Right Click", "Place A Block");
    printInstruction("Left Click", "Removes A Block");

    printInstruction("Move Mouse", "Look");

    printInstruction("ESC", "Exit Game");

    std::cout << "Press Enter to Continue...";
    std::cin.ignore();
}
/**
 * @brief Launches the client
 * @param config Config to be used by the client engine
 * @param launchingJustClient It defines if the instructions should be printed
 * @return int Exit flag (Success, or Failure)
 */
int launchClient(const ClientConfig &config, bool launchingJustClient)
{
    if (launchingJustClient && config.shouldShowInstructions) {
        printInstructions();
    }
    LOG("Launcher", "Launching client");
    switch (runClientEngine(config)) {
        case EngineStatus::Exit:
        case EngineStatus::Ok:
            return exitSuccess();

        case EngineStatus::ExitServerDisconnect:
            return exitSuccess("Client was disconnected from the server.");

        case EngineStatus::ExitServerTimeout:
            return exitSuccess(
                "Server timeout, client forcefully was disconnected.");

        case EngineStatus::GLInitError:
            return exitFailure("OpenGL failed to initilise correctly");

        case EngineStatus::CouldNotConnect:
            return exitFailure("Connection to server could not be established");
    }

    return exitFailure("Unknown error");
}

/**
 * @brief Launches both the client and the server
 * @param config The config to be used by client/server engines
 * @return int Exit flag (Success, or Failure)
 */
int launchBoth(const Config &config)
{
    ServerLauncher server(config.server, sf::milliseconds(5000));
    std::thread serverThread([&server]() {
        LOG("Launcher", "Launching server");
        server.runServerEngine();
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    int exit = launchClient(config.client, false);
    serverThread.join();
    return exit;
}

/**
 * @brief Launches 2 clients and the server. Useful for testing multiplayer
 * @param config The config to be used by client/server engines
 * @return int Exit flag (Success, or Failure)
 */
int launchServerAnd2Players(const Config &config)
{
    ServerLauncher server(config.server, sf::milliseconds(5000));
    std::thread serverThread([&server]() {
        LOG("Launcher", "Launching server");
        server.runServerEngine();
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::thread client2(launchClient, config.client, false);

    int exit = launchClient(config.client, false);

    client2.join();
    serverThread.join();
    return exit;
}
} // namespace

int main(int argc, char **argv)
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

    switch (config.launchType) {
        case LaunchType::Both:
            return launchBoth(config);

        case LaunchType::Server:
            return launchServer(config.server);

        case LaunchType::Client:
            return launchClient(config.client, true);

        case LaunchType::TwoPlayer:
            return launchServerAnd2Players(config);
    }

    enet_deinitialize();
}
