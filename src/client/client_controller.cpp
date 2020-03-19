#include "client_controller.h"
#include "game.h"
#include "lua/client_lua_callback.h"

using State = ClientStateControl::StateId;

class CreateWorldAction final : public ClientStateControl::ControlAction {
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

void ClientStateControl::createWorld(const std::string& name, const std::string& seed)
{
    if (currentState == StateId::InMenu) {
        currentState = StateId::CreateGame;
        paramA = name;
        paramB = seed;
    }
}

void ClientStateControl::loadWorld(const std::string& name)
{
    if (currentState == StateId::InMenu) {
        currentState = StateId::LoadGame;
        paramA = name;
    }
}

void ClientStateControl::joinWorld(const std::string& ipAddress)
{
    if (currentState == StateId::InMenu) {
        currentState = StateId::JoinGame;
        paramA = ipAddress;
    }
}

void ClientStateControl::pauseGame()
{
    if (currentState == StateId::InGame) {
        currentState = StateId::Paused;
    }
}

void ClientStateControl::resumeGame()
{
    if (currentState == StateId::Paused) {
        currentState = StateId::InGame;
    }
}

void ClientStateControl::exitGame()
{
    if (currentState == StateId::Paused || currentState == StateId::InGame) {
        currentState = StateId::ExitGame;
    }
}

void ClientStateControl::shutdown()
{
    currentState = StateId::Shutdown;
}