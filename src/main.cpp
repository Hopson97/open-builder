#include <common/constants.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

#include "client/engine.h"
#include "server/application.h"

#include "client/client_config.h"
#include "server/server_config.h"

namespace {
    enum class LaunchType {
        Server,
        Client,
        Both,
    };

    struct Config {
        LaunchType launchType;

        server::Config serverOptions;
        client::Config clientOptions;
    };

    void loadFromConfigFile(Config &config)
    {
        std::ifstream inFile("config.txt");
        std::string line;
        int option;
        while (inFile >> line >> option) {
            if (line == "FULLSCREEN") {
                config.clientOptions.fullScreen = option;
            }
            else if (line == "WIN_WIDTH") {
                config.clientOptions.windowWidth = option;
            }
            else if (line == "WIN_HEIGHT") {
                config.clientOptions.windowHeight = option;
            }
            else if (line == "FPS_CAPPED") {
                config.clientOptions.isFpsCapped = option;
            }
            else if (line == "FPS") {
                config.clientOptions.fpsLimit = option;
            }
            else if (line == "FOV") {
                config.clientOptions.fov = option;
            }
        }
    }

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
                    config.serverOptions.maxConnections = 4;
                }
            }
            else if (option.first == "-client") {
                config.launchType = LaunchType::Client;
            }
        }
    }

    int exitSuccess()
    {
        std::cout << "Engine exited successfully." << std::endl;
        return EXIT_SUCCESS;
    }

    int exitFailure(const char *message)
    {
        std::cerr << "Engine exited with error: \"" << message << "\"."
                  << std::endl;
        return EXIT_FAILURE;
    }

    int launchServer(const server::Config &config,
                     sf::Time timeout = sf::seconds(8))
    {
        std::cout << "Launching server.\n";
        server::runServerApp(config, timeout);
        return EXIT_SUCCESS;
    }

    int launchClient(const client::Config &config)
    {
        std::cout << "Launching client.\n";
        switch (client::runClientEngine(config)) {
            case client::EngineStatus::Exit:
            case client::EngineStatus::Ok:
                return exitSuccess();

            case client::EngineStatus::GLInitError:
                return exitFailure("OpenGL failed to initilise correctly");
        }
        return exitFailure("Unknown error");
    }

    int launchBoth(const Config &config)
    {
        std::thread serverThread(launchServer, config.serverOptions,
                                 sf::milliseconds(256));
        int exit = launchClient(config.clientOptions);

        std::cout << "Awaiting server shutdown.\n";
        serverThread.join();
        return exit;
    }
} // namespace

int main(int argc, char **argv)
{
    Config config;

    std::vector<std::pair<std::string, std::string>> args;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argc > i + 1) {
            args.emplace_back(argv[i], argv[i + 1]);
        }
    }

    parseArgs(config, args);
    loadFromConfigFile(config);

    switch (config.launchType) {
        case LaunchType::Both:
            return launchBoth(config);

        case LaunchType::Server:
            return launchServer(config.serverOptions);
            break;

        case LaunchType::Client:
            return launchClient(config.clientOptions);
    }
}