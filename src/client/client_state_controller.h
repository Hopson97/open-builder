#pragma once

#include <memory>
#include <string>

class Game;
class ClientLuaCallbacks;

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
    };

    class ControlAction {
      public:
        virtual bool executeAction(Game& game, StateId& currentState,
                                   ClientLuaCallbacks& callbacks) = 0;
    };

    // Lua API functions
    void createWorld(const std::string& name, const std::string& seed);
    void loadWorld(const std::string& name);
    void joinWorld(const std::string& ipAddress);
    void pauseGame();
    void resumeGame();
    void exitGame();
    void shutdown();

    // C++ API Functions
    bool executeAction(Game& game, ClientLuaCallbacks& callbacks);

    StateId currentState() const;

  private:
    StateId m_currentState = StateId::InMenu;
    std::unique_ptr<ControlAction> m_nextAction;
};
