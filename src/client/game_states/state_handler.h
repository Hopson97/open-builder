#pragma once

#include <memory>
#include <stack>

#include "game_state.h"

namespace client {
    /**
     * @brief Manager class for the "state" of the game, eg MainMenu, Survival,
     * etc
     *
     */
    class StateHandler final {
      public:
        StateHandler();

        /// Gets the current game state
        GameState &peekState();

        /// Pushes a new state to the state handler
        template <typename State, typename... Args>
        void pushState(Args &&... args);

        /// Removes the top/current game state
        void popState();

        void update();
        void clear();

        bool isEmpty() const;

      private:
        std::stack<std::unique_ptr<GameState>> m_gameStates;
        int m_popCount = 0;
    };

    template <typename State, typename... Args>
    void StateHandler::pushState(Args &&... args)
    {
        m_gameStates.push(std::make_unique<State>(std::forward<Args>(args)...));
        peekState().onEnter();
    }
} // namespace client
