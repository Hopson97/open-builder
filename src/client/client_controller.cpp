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