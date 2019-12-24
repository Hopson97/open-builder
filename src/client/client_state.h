#pragma once

#include "engine_status.h"
#include "maths.h"
#include <array>
#include <common/world/chunk.h>

struct Entity final {
    glm::vec3 position{0.0f, 0.0f, 12.0f}, rotation{0.0f};
    bool active = false;
};

struct ClientState {
    std::array<Entity, 512> entities;
    ChunkPositionMap<Chunk *> chunks;
    ChunkManager chunkManager;

    EngineStatus status = EngineStatus::Ok;
};