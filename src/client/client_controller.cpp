#include "client_controller.h"

#include "client_config.h"
//
//  Actions
//
bool ClientController::Action::isEmptyAction()
{
    return false;
}

// Empty
bool ClientController::NoAction::isEmptyAction()
{
    return true;
}

void ClientController::NoAction::apply()
{
}

ClientGame ClientController::NoAction::finish()
{
    return {};
}

// Create World
ClientController::CreateWorldAction::CreateWorldAction()
{
}

void ClientController::CreateWorldAction::apply()
{
    // Create Server which creates a world I guess
    // Connect to server
    // ????????
}

ClientGame ClientController::CreateWorldAction::finish()
{
    ClientGame game;
    game.serverThread = std::make_unique<std::thread>(
        [this, &game] { game.serverLauncher->runServerEngine(); });

    game.client = std::make_unique<Client>();
    ClientConfig c;

    if (!game.client->init(c, 1600.0f / 900.0f)) {
        game.createStatus = EngineStatus::CouldNotConnect;
        return game;
    }

    return game;
}

//
//  Client Controller
//

ClientController::ClientController()
{
    resetAction();
}

ClientController::~ClientController()
{
    if (m_actionApplyThread && m_actionApplyThread->joinable()) {
        m_actionApplyThread->join();
    }
}

void ClientController::createWorld()
{
    if (m_actionState == ActionState::Idle && m_clientState == ClientState::Menu) {
        m_currentAction = std::make_unique<CreateWorldAction>();
    }
}

void ClientController::applyAction()
{
    if (m_actionState == ActionState::Idle && !m_currentAction->isEmptyAction()) {
        m_actionApplyThread = std::make_unique<std::thread>([this] {
            m_actionState = ActionState::Working;
            m_currentAction->apply();
            m_actionState = ActionState::Done;
        });
    }
}

bool ClientController::isActionComplete()
{
    return m_actionState == ActionState::Done;
}

ClientGame ClientController::completeAction()
{
    if (m_actionState == ActionState::Done) {
        m_actionApplyThread->join();
        auto result = m_currentAction->finish();
        resetAction();
        return result;
    }

    return {};
}

void ClientController::resetAction()
{
    m_currentAction = std::make_unique<NoAction>();
}

#include "engine_status.h"

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