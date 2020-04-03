#include "state_manager.h"

#include <cassert>

ClientGameState::ClientGameState(StateManager& manager)
    : mp_stateManager(&manager)
{
}

void StateManager::push(gui::GuiSystem& gui, std::unique_ptr<ClientGameState> gameState)
{
    m_pendingState = std::move(gameState);
    m_pendingAction = Action::Push;
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

void StateManager::updateStack(gui::GuiSystem& gui)
{
    if (m_pendingAction == Action::Pop && !m_stateStack.empty()) {
        assert(mp_top);
        mp_top->onShutdown();
        m_stateStack.pop();
        updateTop(gui);
    }
    else if (m_pendingAction == Action::Push) {
        m_stateStack.emplace(std::move(m_pendingState));
        updateTop(gui);
    }
    m_pendingAction = Action::None;
}

void StateManager::updateTop(gui::GuiSystem& gui)
{
    if (!isEmpty()) {
        mp_top = m_stateStack.top().get();
        mp_top->onStart(gui);
    }
}
