#pragma once

/// The main "core" of the game, wherethe main loop lives and the states
/// are processed
namespace client {

    struct Config;

    enum class EngineStatus {
        Ok = 10,
        Exit = 11,

        GLInitError = 20,
    };

    EngineStatus runClientEngine(const Config &config);
} // namespace client