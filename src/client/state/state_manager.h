#pragma once

#include <common/macros.h>
#include <memory>
#include <stack>

class StateManager;
class Keyboard;

namespace gui {
    class GuiSystem;
}

class ClientGameState {
  public:
    ClientGameState(StateManager& manager);
    virtual ~ClientGameState() = default;

    virtual void input(){};
    virtual void update(){};
    virtual void render(){};

    virtual void onStart(gui::GuiSystem& gui) = 0;
    virtual void onShutdown() = 0;

  protected:
    StateManager* mp_stateManager = nullptr;

    NON_COPYABLE(ClientGameState)
};

class StateManager {
  public:
    void push(gui::GuiSystem& gui, std::unique_ptr<ClientGameState> gameState);
    void pop();

    void handleInput();
    void handleUpdate();
    void handleRender();

    bool isEmpty() const;

    // Must be called at the end of each frame of
    // the game
    void updateStack(gui::GuiSystem& gui);

  private:
    void updateTop(gui::GuiSystem& gui);

    enum class Action {
        Push,
        Pop,
        None,
    };
    std::unique_ptr<ClientGameState> m_pendingState;
    Action m_pendingAction = Action::None;

    std::stack<std::unique_ptr<ClientGameState>> m_stateStack;
    ClientGameState* mp_top = nullptr;
};