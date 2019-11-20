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
                m_gameStates.pop();
            }
        }
    }

    bool StateHandler::isEmpty() const 
    {
        return m_gameStates.empty();
    }
    // clang-format on 
} // namespace client