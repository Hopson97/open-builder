#include <common/debug.h>
#include <cstdlib>
#include <fstream>
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
    config.client.fpsLimit = std::stoi(clientData["fps_limit"]);
    config.client.fov = std::stoi(clientData["fov"]);
    config.client.fpsLimit = std::stoi(clientData["fps_limit"]);
    config.client.connectionTimeout = sf::milliseconds(std::stoi(clientData["connection_timeout"]));
    config.client.skinName = clientData["skin"];
    config.client.texturePack = clientData["texture_pack"];

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
    std::cout << "Engine exited successfully.\"" << message << "\"."
              << std::endl;
    return EXIT_SUCCESS;
}

/**
 * @brief Prints failure message
 * @return int Exit failure flag
 */
int exitFailure(const char *message)
{
    std::cerr << "Engine exited with error: \"" << message << "\"."
              << std::endl;
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
    runServerEngine(config, timeout);
    LOG("Launcher", "Server has exited.");
    return EXIT_SUCCESS;
}

/**
 * @brief Launches the client
 * @param config Config to be used by the client engine
 * @return int Exit flag (Success, or Failure)
 */
int launchClient(const ClientConfig &config)
{
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
    std::thread serverThread(launchServer, config.server,
                             sf::milliseconds(5000));
    //temp, the retrying until connection made doesn't seem to be working
    std::this_thread::sleep_for(std::chrono::milliseconds(700));
    int exit = launchClient(config.client);
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
    std::thread serverThread(launchServer, config.server,
                             sf::milliseconds(20000));

    std::thread client2(launchClient, config.client);

    int exit = launchClient(config.client);

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
            return launchClient(config.client);

        case LaunchType::TwoPlayer:
            return launchServerAnd2Players(config);
    }

    enet_deinitialize();
}