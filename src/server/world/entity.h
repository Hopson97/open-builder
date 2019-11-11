#pragma once

#include <array>
#include <common/network/constants.h>
#include <common/types.h>
#include <common/util/maths.h>

namespace server {
    /// Per-Instance information about the entities of the world
    struct Entity {
      public:
        void moveForwards();
        void moveLeft();
        void moveRight();
        void moveBackwards();

        glm::vec3 velocity{0.0f};
        glm::vec3 position{10.0f, 32.0f, 10.0f};
        glm::vec2 rotation{0.0f};

        float speed = 0.5;

        // This might cause padding issues, interfering with cache, but is fine
        // for now
        bool isAlive = false;
    };

    using EntityArray = std::array<Entity, ENTITY_COUNT>;
} // namespace server
