#pragma once

#include <SFML/Window/Event.hpp>

namespace client {
    class StateHandler;
    class Keyboard;
    class Renderer;
    class Camera;

    /// Base class for the different game states of open-builder
    class GameState {
      public:
        GameState(StateHandler &stateHandler);
        virtual ~GameState() = default;

        virtual void handleKeyDown([[maybe_unused]] sf::Keyboard::Key key)
        {
        }

        virtual void handleKeyUp([[maybe_unused]] sf::Keyboard::Key key)
        {
        }

        virtual void handleInput(const Keyboard &keyboard,
                                 const sf::Window &window) = 0;
        virtual void update(Camera &camera) = 0;
        virtual void render(Renderer &renderer) = 0;

      protected:
        StateHandler *mp_stateHandler;
    };
} // namespace client
