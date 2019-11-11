#pragma once

#include <SFML/Window/Window.hpp>
#include <memory>

#include "game_states/state_handler.h"
#include "input/keyboard.h"
#include "renderer/camera.h"
#include "renderer/renderer.h"
#include "util/fps_counter.h"

struct LaunchConfig;

namespace client {
    /// The main "core" of the game, wherethe main loop lives and the states
    /// are processed
    class Engine final {
      public:
        /// The status of the engine at any given time
        enum class Status {
            Ok = 10,
            Exit = 11,

            GLInitError = 20,
        };

        Engine(const LaunchConfig &config);
        Status runClient();

      private:
        void handleWindowEvents();
        void handleInput();
        void update();
        void render();

        void exit();

        sf::Window m_window;
        Keyboard m_keyboard;
        Status m_status;

        FPSCounter m_fpsCounter;

        StateHandler m_stateHandler;
        GameState *mp_currentState = nullptr;

        std::unique_ptr<Renderer> m_renderer;
        Camera m_camera;
    };
} // namespace client