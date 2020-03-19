#pragma once

#include <memory>
#include <string>

class Game;
class ClientLuaCallbacks;
struct ClientConfig;

/**
 * @brief Provides a Lua API for controlling the state of the client
 *
 */
class ClientStateController {
  public:
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

    class ControlAction {
      public:
        virtual bool executeAction(ClientConfig& config, Game& game,
                                   StateId& currentState,
                                   ClientLuaCallbacks& callbacks) = 0;
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
