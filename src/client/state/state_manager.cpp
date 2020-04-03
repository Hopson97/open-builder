#include "state_manager.h"

#include <cassert>

ClientGameState::ClientGameState(StateManager& manager)
    : mp_stateManager(&manager)
{
}

void StateManager::forceState(std::unique_ptr<ClientGameState> gameState)
{
    m_stateStack.emplace(std::move(gameState));
    updateTop();
    mp_top->onStart();
}

void StateManager::push(std::unique_ptr<ClientGameState> gameState)
{
    m_pendingAction = Action::Push;
    m_pendingState = std::move(gameState);
}

void StateManager::pop()
{
    m_pendingAction = Action::Pop;
}

void StateManager::handleInput()
{
    assert(mp_top);
    mp_top->input();
}

void StateManager::handleUpdate()
{
    assert(mp_top);
    mp_top->update();
}

void StateManager::handleRender()
{
    assert(mp_top);
    mp_top->render();
}

void StateManager::updateStack()
{
    if (m_pendingAction == Action::Pop && !m_stateStack.empty()) {
        assert(mp_top);
        mp_top->onShutdown();
        m_stateStack.pop();
        updateTop();
    }
    else if (m_pendingAction == Action::Push) {
        forceState(std::move(m_pendingState));
    }
    m_pendingAction = Action::None;
}

void StateManager::updateTop()
{
    mp_top = m_stateStack.top().get();
}
