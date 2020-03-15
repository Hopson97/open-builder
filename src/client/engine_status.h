#pragma once

#include <string>

/**
 * @brief Status information about the engine
 */
enum class EngineStatus {
    Ok = 10,
    Exit = 11,
    ExitServerDisconnect = 12,
    ExitServerTimeout = 13,

    GLInitError = 20,
    CouldNotConnect = 21,
};

struct ClientStateControl {
    enum class StateId {
        // Absolute States
        InMenu,
        InGame,
        Paused,

        // Transition States
        CreateGame,
        LoadGame,
        JoinGame,
        ExitGame,
        Shutdown,
    };

    StateId currentState = StateId::InMenu;
    std::string paramA;
    std::string paramB;

    void createWorld(const std::string& name, const std::string& seed);
    void loadWorld(const std::string& name);
    void joinWorld(const std::string& ipAddress);

    void pauseGame();
    void resumeGame();
    void exitGame();
    void shutdown();
};
