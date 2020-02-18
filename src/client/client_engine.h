#pragma once

struct ClientConfig;

#include "engine_status.h"

/**
 * @brief Run the client
 * @param config
 * @return EngineStatus The status of the engine on exit
 */
EngineStatus runClientEngine(const ClientConfig& config);