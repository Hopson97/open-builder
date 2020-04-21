#include "client_state_controller.h"
#include "game/game.h"
#include "game/game_type.h"
#include "lua/client_lua_callback.h"

namespace {
    using State = ClientStateController::StateId;

    class CreateWorldAction final : public ClientStateController::ControlAction {
      public:
        CreateWorldAction(const std::string& name, const std::string& seed)
            : m_worldName(name)
            , m_worldSeed(seed)
        {
        }

        bool executeAction(Game& game, State& m_currentState,
                           ClientLuaCallbacks& callbacks) final override
        {
            game.setGameDefintion<LocalGame>("Test");
            if (game.start()) {
                callbacks.onEnterGame();
                m_currentState = State::InGame;
            }
            else {
                callbacks.onError("Failed to create world.");
                m_currentState = State::InMenu;
            }
            return true;
        }

      private:
        const std::string m_worldName;
        const std::string m_worldSeed;
    };

    class LoadWorldActon final : public ClientStateController::ControlAction {
      public:
        LoadWorldActon(const std::string& name)
            : m_worldName(name)
        {
        }

        bool executeAction(Game& game, State& m_currentState,
                           ClientLuaCallbacks& callbacks) final override
        {
            game.setGameDefintion<LocalGame>("Test");
            if (game.start()) {
                callbacks.onEnterGame();
                m_currentState = State::InGame;
            }
            else {
                callbacks.onError("Failed to load world.");
                m_currentState = State::InMenu;
            }
            return true;
        }

      private:
        const std::string m_worldName;
    };

    class JoinServerAction final : public ClientStateController::ControlAction {
      public:
        JoinServerAction(const std::string& serverIp)
            : m_serverIp(serverIp)
        {
        }

        bool executeAction(Game& game, State& m_currentState,
                           ClientLuaCallbacks& callbacks) final override
        {
            game.setGameDefintion<RemoteGame>(m_serverIp);
            if (game.start()) {
                callbacks.onEnterGame();
                m_currentState = State::InGame;
            }
            else {
                callbacks.onError("Failed to join world.");
                m_currentState = State::InMenu;
            }
            return true;
        }

      private:
        const std::string m_serverIp;
    };

    class ExitGameAction final : public ClientStateController::ControlAction {
      public:
        bool executeAction(Game& game, State& m_currentState,
                           ClientLuaCallbacks& callbacks) final override
        {
            game.shutdown();
            callbacks.onExitGame();
            m_currentState = State::InMenu;
            return true;
        }
    };

    class ShutdownGameAction final : public ClientStateController::ControlAction {
      public:
        bool executeAction(Game&, State&, ClientLuaCallbacks&) final override
        {
            return false;
        }
    };
} // namespace

void ClientStateController::createWorld(const std::string& name, const std::string& seed)
{
    if (m_currentState == StateId::InMenu) {
        m_nextAction = std::make_unique<CreateWorldAction>(name, seed);
    }
}

void ClientStateController::loadWorld(const std::string& name)
{
    if (m_currentState == StateId::InMenu) {
        m_nextAction = std::make_unique<LoadWorldActon>(name);
    }
}

void ClientStateController::joinWorld(const std::string& ipAddress)
{
    if (m_currentState == StateId::InMenu) {
        m_nextAction = std::make_unique<JoinServerAction>(ipAddress);
    }
}

void ClientStateController::pauseGame()
{
    if (m_currentState == StateId::InGame) {
        m_currentState = StateId::Paused;
    }
}

void ClientStateController::resumeGame()
{
    if (m_currentState == StateId::Paused) {
        m_currentState = StateId::InGame;
    }
}

void ClientStateController::exitGame()
{
    if (m_currentState == StateId::Paused || m_currentState == StateId::InGame) {
        m_nextAction = std::make_unique<ExitGameAction>();
    }
}

void ClientStateController::shutdown()
{
    m_nextAction = std::make_unique<ShutdownGameAction>();
}

bool ClientStateController::executeAction(Game& game, ClientLuaCallbacks& callbacks)
{
    if (m_nextAction) {
        bool result = m_nextAction->executeAction(game, m_currentState, callbacks);
        m_nextAction.reset();
        return result;
    }
    return true;
}

State ClientStateController::currentState() const
{
    return m_currentState;
}
