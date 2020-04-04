#pragma once

#include "../maths.h"

struct Camera {
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};

    ViewFrustum m_frustum{};
    glm::mat4 m_projection{1.0f};

    static Camera createCamera(const glm::vec3& position);
};