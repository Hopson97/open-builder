#pragma once

#include <glm/vec3.hpp>

struct EntityState {
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};

    glm::vec3 lastPosition{0.0f};
    glm::vec3 lastRotation{0.0f};

    glm::vec3 velocity{0.0};

    bool active = false;
};
