#pragma once

#include <SFML/Window/Window.hpp>
#include <memory>

#include "game_states/state_handler.h"
#include "input/keyboard.h"
#include "renderer/camera.h"
#include "renderer/renderer.h"
#include "util/fps_counter.h"

struct LaunchConfig;

/// The main "core" of the game, wherethe main loop lives and the states
/// are processed
namespace client {

    enum class EngineStatus {
        Ok = 10,
        Exit = 11,

        GLInitError = 20,
    };

    EngineStatus runClientEngine(const LaunchConfig &config);
} // namespace client