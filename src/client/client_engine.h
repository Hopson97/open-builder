#pragma once

struct ClientConfig;

enum class EngineStatus {
    Ok = 10,
    Exit = 11,

    GLInitError = 20,
};

EngineStatus runClientEngine(const ClientConfig &config);