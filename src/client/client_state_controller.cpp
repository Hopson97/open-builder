#include "client_state_controller.h"
#include "game.h"
#include "lua/client_lua_callback.h"

using State = ClientStateController::StateId;

class CreateWorldAction final : public ClientStateController::ControlAction {
  public:
    CreateWorldAction(const std::string& name, const std::string& seed)
        : m_worldName(name)
        , m_worldSeed(seed)
    {
    }

    bool executeAction(ClientConfig& config, Game& game, State& currentState,
                       ClientLuaCallbacks& callbacks) final override
    {
        if (game.initGame(config)) {
            callbacks.onEnterGame();
            currentState = State::InGame;
        }
    }

  private:
    const std::string m_worldName;
    const std::string m_worldSeed;
};

void ClientStateController::createWorld(const std::string& name, const std::string& seed)
{
    if (currentState == StateId::InMenu) {
        currentState = StateId::CreateGame;
        paramA = name;
        paramB = seed;
    }
}

void ClientStateController::loadWorld(const std::string& name)
{
    if (currentState == StateId::InMenu) {
        currentState = StateId::LoadGame;
        paramA = name;
    }
}

void ClientStateController::joinWorld(const std::string& ipAddress)
{
    if (currentState == StateId::InMenu) {
        currentState = StateId::JoinGame;
        paramA = ipAddress;
    }
}

void ClientStateController::pauseGame()
{
    if (currentState == StateId::InGame) {
        currentState = StateId::Paused;
    }
}

void ClientStateController::resumeGame()
{
    if (currentState == StateId::Paused) {
        currentState = StateId::InGame;
    }
}

void ClientStateController::exitGame()
{
    if (currentState == StateId::Paused || currentState == StateId::InGame) {
        currentState = StateId::ExitGame;
    }
}

void ClientStateController::shutdown()
{
    currentState = StateId::Shutdown;
}