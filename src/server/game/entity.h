#pragma once

#include <array>
#include <common/types.h>
#include <common/util/maths.h>
#include <common/network/constants.h>

namespace server {
    /// Per-Instance information about the entities of the world
    struct Entity {
        maths::Transform transform;
        Entity *p_target = nullptr;
        glm::vec3 targetPoint;
        glm::vec3 velocity{0.0f};

        u8 alive = 0;
    };

    using EntityArray = std::array<Entity, ENTITY_COUNT>;
} // namespace server
