#include "state_handler.h"

namespace client {
    // clang-format off
    StateHandler::StateHandler() 
    {
        
    }

    GameState &StateHandler::peekState() 
    { 
        return *m_gameStates.top(); 
    }

    void StateHandler::popState() 
    {
        m_popCount++;
    }

    void StateHandler::update()
    {
        while (m_popCount > 0) {
            m_popCount--;
            if (!isEmpty()) {
                peekState().onExit();
                m_gameStates.pop();
            }
        }
        if (!isEmpty()) {
            peekState().onEnter();
        }
    }

    void StateHandler::clear() {
        while(!isEmpty()) {
            peekState().onExit();
            m_gameStates.pop();
        }
    }

    bool StateHandler::isEmpty() const 
    {
        return m_gameStates.empty();
    }
    // clang-format on 
} // namespace client