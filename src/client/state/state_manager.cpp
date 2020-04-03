#include "state_manager.h"

#include <cassert>

ClientGameState::ClientGameState(StateManager& manager)
    : mp_stateManager(&manager)
{
}

void StateManager::push(gui::GuiSystem& gui, std::unique_ptr<ClientGameState> gameState)
{
    m_stateStack.emplace(std::move(gameState));
    updateTop();
    mp_top->onStart(gui);
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

bool StateManager::isEmpty() const
{
    return m_stateStack.empty();
}

void StateManager::updateStack()
{
    if (m_pendingAction == Action::Pop && !m_stateStack.empty()) {
        assert(mp_top);
        mp_top->onShutdown();
        m_stateStack.pop();
        updateTop();
    }
    m_pendingAction = Action::None;
}

void StateManager::updateTop()
{
    if (!isEmpty()) {
        mp_top = m_stateStack.top().get();
    }
}
