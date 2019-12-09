#pragma once

struct ClientConfig;

/**
 * @brief Status information about the engine
 */
enum class EngineStatus {
    Ok = 10,
    Exit = 11,

    GLInitError = 20,
};

/**
 * @brief Run the client
 * @param config
 * @return EngineStatus The status of the engine on exit
 */
EngineStatus runClientEngine(const ClientConfig &config);