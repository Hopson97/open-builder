#pragma once

#include <stack>
#include <memory>
#include <common/macros.h>

class StateManager;
class Keyboard;

class ClientGameState {
  public:
    ClientGameState(StateManager& manager);
    virtual ~ClientGameState() = default;

    virtual void input() = 0;
    virtual void update() = 0;
    virtual void render() = 0;

    virtual void onStart() = 0;
    virtual void onShutdown() = 0;

  protected:
    StateManager* mp_stateManager = nullptr;

    NON_COPYABLE(ClientGameState)
};

class StateManager {
  public:
    void forceState(std::unique_ptr<ClientGameState> gameState);
    void push(std::unique_ptr<ClientGameState> gameState);
    void pop();

    void handleInput();
    void handleUpdate();
    void handleRender();
        
    // Must be called at the end of each frame of
    // the game
    void updateStack();
    
  private:
    void updateTop();

    enum class Action {
        Push, Pop, None,
    };
    std::unique_ptr<ClientGameState> m_pendingState;
    Action m_pendingAction = Action::None;

    std::stack<std::unique_ptr<ClientGameState>> m_stateStack;
    ClientGameState* mp_top = nullptr;
};