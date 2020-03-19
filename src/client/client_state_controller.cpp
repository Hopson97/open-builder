#include "client_state_controller.h"
#include "game.h"
#include "lua/client_lua_callback.h"

namespace {
    using State = ClientStateController::StateId;

    /// = = = = = = = = = = = = =
    ///     CreateWorldAction
    /// = = = = = = = = = = = = =
    class CreateWorldAction final : public ClientStateController::ControlAction {
      public:
        CreateWorldAction(const std::string& name, const std::string& seed)
            : m_worldName(name)
            , m_worldSeed(seed)
        {
        }

        bool executeAction(const ClientConfig& config, Game& game, State& currentState,
                           ClientLuaCallbacks& callbacks) final override
        {
            if (game.initGame(config)) {
                callbacks.onEnterGame();
                currentState = State::InGame;
            }
            else {
                callbacks.onError("Failed to create world.");
                currentState = State::InMenu;
            }
            return true;
        }

      private:
        const std::string m_worldName;
        const std::string m_worldSeed;
    };

    /// = = = = = = = = = = = = =
    ///     LoadWorldActon
    /// = = = = = = = = = = = = =
    class LoadWorldActon final : public ClientStateController::ControlAction {
      public:
        LoadWorldActon(const std::string& name)
            : m_worldName(name)
        {
        }

        bool executeAction(const ClientConfig& config, Game& game, State& currentState,
                           ClientLuaCallbacks& callbacks) final override
        {
            if (game.initGame(config)) {
                callbacks.onEnterGame();
                currentState = State::InGame;
            }
            else {
                callbacks.onError("Failed to load world.");
                currentState = State::InMenu;
            }
            return true;
        }

      private:
        const std::string m_worldName;
    };

    /// = = = = = = = = = = = = =
    ///     JoinServerAction
    /// = = = = = = = = = = = = =
    class JoinServerAction final : public ClientStateController::ControlAction {
      public:
        JoinServerAction(const std::string& serverIp)
            : m_serverIp(serverIp)
        {
        }

        bool executeAction(const ClientConfig& config, Game& game, State& currentState,
                           ClientLuaCallbacks& callbacks) final override
        {
            if (game.initGame(config, m_serverIp)) {
                callbacks.onEnterGame();
                currentState = State::InGame;
            }
            else {
                callbacks.onError("Failed to join world.");
                currentState = State::InMenu;
            }
            return true;
        }

      private:
        const std::string m_serverIp;
    };

    /// = = = = = = = = = = = = =
    ///     ExitGameAction
    /// = = = = = = = = = = = = =
    class ExitGameAction final : public ClientStateController::ControlAction {
      public:
        bool executeAction(const ClientConfig&, Game& game, State& currentState,
                           ClientLuaCallbacks& callbacks) final override
        {
            game.stopGame();
            callbacks.onExitGame();
            currentState = State::InMenu;
            return true;
        }
    };

    /// = = = = = = = = = = = = =
    ///     ShutdownGameAction
    /// = = = = = = = = = = = = =
    class ShutdownGameAction final : public ClientStateController::ControlAction {
      public:
        bool executeAction(const ClientConfig&, Game&, State&,
                           ClientLuaCallbacks&) final override
        {
            return false;
        }
    };
} // namespace

void ClientStateController::createWorld(const std::string& name, const std::string& seed)
{
    if (currentState == StateId::InMenu) {
        m_nextAction = std::make_unique<CreateWorldAction>(name, seed);
    }
}

void ClientStateController::loadWorld(const std::string& name)
{
    if (currentState == StateId::InMenu) {
        m_nextAction = std::make_unique<LoadWorldActon>(name);
    }
}

void ClientStateController::joinWorld(const std::string& ipAddress)
{
    if (currentState == StateId::InMenu) {
        m_nextAction = std::make_unique<JoinServerAction>(ipAddress);
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
        m_nextAction = std::make_unique<ExitGameAction>();
    }
}

void ClientStateController::shutdown()
{
    m_nextAction = std::make_unique<ShutdownGameAction>();
}

bool ClientStateController::executeAction(const ClientConfig& config, Game& game,
                                          ClientLuaCallbacks& callbacks)
{
    if (m_nextAction) {
        bool result = m_nextAction->executeAction(config, game, currentState, callbacks);
        m_nextAction.release();
        return result;
    }
    return true;
}