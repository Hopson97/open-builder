#pragma once

#include <string>

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
