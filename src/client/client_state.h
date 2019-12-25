#pragma once

#include "engine_status.h"
#include "maths.h"
#include "world/client_chunk_manager.h"
#include <array>
#include <common/world/chunk_manager.h>
#include <unordered_set>

struct Entity final {
    glm::vec3 position{0.0f, 0.0f, 12.0f}, rotation{0.0f};
    bool active = false;
};

struct ClientState {
    std::array<Entity, 512> entities;
    ClientChunkManager chunkManager;

    EngineStatus status = EngineStatus::Ok;
};