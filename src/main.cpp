#include <common/launch_config.h>
#include <common/network/constants.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

#include "client/engine.h"
#include "server/application.h"

namespace {
    void loadFromConfigFile(LaunchConfig &config)
    {
        std::ifstream inFile("config.txt");
        std::string line;
        int option;
        while (inFile >> line >> option) {
            if (line == "FULLSCREEN") {
                config.graphicOptions.fullScreen = option;
            }
            else if (line == "WIN_WIDTH") {
                config.graphicOptions.windowWidth = option;
            }
            else if (line == "WIN_HEIGHT") {
                config.graphicOptions.windowHeight = option;
            }
            else if (line == "FPS_CAPPED") {
                config.graphicOptions.isFpsCapped = option;
            }
            else if (line == "FPS") {
                config.graphicOptions.fpsLimit = option;
            }
            else if (line == "FOV") {
                config.graphicOptions.fov = option;
            }
        }
    }

    void parseArgs(LaunchConfig &config,
                   const std::vector<std::pair<std::string, std::string>> &args)
    {
        for (const auto &option : args) {
            // Set launch type to be server.
            // Option: MAX_CONNECTIONS 2-16
            if (option.first == "-server") {
                config.type = LaunchConfig::Type::Server;
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
                config.type = LaunchConfig::Type::Client;
            }
        }
    }

    auto loadConfig(int argc, char **argv)
    {
        LaunchConfig config;

        std::vector<std::pair<std::string, std::string>> args;
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] == '-' && argc > i + 1) {
                args.emplace_back(argv[i], argv[i + 1]);
            }
        }

        parseArgs(config, args);
        loadFromConfigFile(config);

        return config;
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

    int launchServer(const LaunchConfig &config,
                     sf::Time timeout = sf::seconds(8))
    {
        std::cout << "Launching server.\n";
        server::Application server(config, PORT);
        server.run(timeout);
        return EXIT_SUCCESS;
    }

    int launchClient(const LaunchConfig &config)
    {
        std::cout << "Launching client.\n";
        client::Engine app(config);
        switch (app.runClient()) {
            case client::Engine::Status::Exit:
            case client::Engine::Status::Ok:
                return exitSuccess();

            case client::Engine::Status::GLInitError:
                return exitFailure("OpenGL failed to initilise correctly");
        }
        return exitFailure("Unknown error");
    }

    int launchBoth(const LaunchConfig &config)
    {
        std::thread serverThread(launchServer, config, sf::milliseconds(500));
        int exit = launchClient(config);

        std::cout << "Awaiting server shutdown.\n";
        serverThread.join();
        return exit;
    }
} // namespace

int main(int argc, char **argv)
{
    auto options = loadConfig(argc, argv);

    switch (options.type) {
        case LaunchConfig::Type::Both:
            return launchBoth(options);

        case LaunchConfig::Type::Server:
            return launchServer(options);
            break;

        case LaunchConfig::Type::Client:
            return launchClient(options);
    }
}