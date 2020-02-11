#include <common/debug.h>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <filesystem>

#include "client/client_engine.h"
#include "server/server_engine.h"

#include "client/client_config.h"
#include "server/server_config.h"

#include <common/network/enet.h>
#include <common/file_io.h>

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
 * @brief Parses the CLI arguments from the user
 * @param config The config to load data into
 * @param args CLI arguments paired as <argument, param>
 */
void parseArgs(ClientConfig& config, LaunchType& launchType,
               const std::vector<std::pair<std::string, std::string>>& args)
{
	// set launch type
    for (const auto& option : args) {
        if (option.first == "-server") {
            launchType = LaunchType::Server;
        }
        else if (option.first == "-client") {
            launchType = LaunchType::Client;
        }
        else if (option.first == "-skin") {
            config.set_skinName(option.second);
        }
    }
}

/**
 * @brief Prints success message
 * @return int Exit success flag
 */
int exitSuccess(const char* message = "Normal exit")
{
    std::cout << "Engine exited successfully.\"" << message << "\"." << '\n';
    return EXIT_SUCCESS;
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

/**
 * @brief Launches the server
 * @param config Config to be used by the server engine
 * @param timeout How long the server waits for a connection before closing
 * @return int Exit success flag
 */
int launchServer(const ServerConfig& config, sf::Time timeout = sf::seconds(8))
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
int launchClient(const ClientConfig& config, bool launchingJustClient)
{
    if (launchingJustClient && config.get_shouldShowInstructions()) {
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
            return exitSuccess("Server timeout, client forcefully was disconnected.");

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
int launchBoth(const ClientConfig& clientConfig, const ServerConfig& serverConfig)
{
    ServerLauncher server(serverConfig, sf::milliseconds(5000));
    std::thread serverThread([&server]() {
        LOG("Launcher", "Launching server");
        server.runServerEngine();
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    int exit = launchClient(clientConfig, false);
    serverThread.join();
    return exit;
}

/**
 * @brief Launches 2 clients and the server. Useful for testing multiplayer
 * @param config The config to be used by client/server engines
 * @return int Exit flag (Success, or Failure)
 */
int launchServerAnd2Players(const ClientConfig& clientConfig, const ServerConfig& serverConfig)
{
    ServerLauncher server(serverConfig, sf::milliseconds(5000));
    std::thread serverThread([&server]() {
        LOG("Launcher", "Launching server");
        server.runServerEngine();
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::thread client2(launchClient, clientConfig, false);

    int exit = launchClient(clientConfig, false);

    client2.join();
    serverThread.join();
    return exit;
}
} // namespace

int main(int argc, char** argv)
{
	// read config files
	ClientConfig clientConfig("client.obd");
	ServerConfig serverConfig("server.obd");

	// parse command line arguments
    std::vector<std::pair<std::string, std::string>> args;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argc > i + 1) {
            args.emplace_back(argv[i], argv[i + 1]);
        }
    }

	LaunchType launchType;
    parseArgs(clientConfig, launchType, args);

	try {
		if (serverConfig.get_maxConnections() < 2) {
			throw std::invalid_argument("Max connections must be at least " +
					std::to_string(MIN_CONNECTIONS) + ".\n");
		}
		else if (serverConfig.get_maxConnections() > 16) {
			throw std::invalid_argument("Max connections must be " +
					std::to_string(MAX_CONNECTIONS) +
					" or below.\n");
		}
	}
	catch (std::invalid_argument& e) {
		std::cout << "Unable to set max connections, defaulting to "
			"4. Reason: "
			<< e.what() << "\n";
		serverConfig.set_maxConnections(4);
	}

    if (enet_initialize() != 0) {
        return exitFailure("Failed to initialise enet");
    }

    switch (launchType) {
        case LaunchType::Both:
            return launchBoth(clientConfig, serverConfig);

        case LaunchType::Server:
            return launchServer(serverConfig);

        case LaunchType::Client:
            return launchClient(clientConfig, true);

        case LaunchType::TwoPlayer:
            return launchServerAnd2Players(clientConfig, serverConfig);
    }

    enet_deinitialize();
}
