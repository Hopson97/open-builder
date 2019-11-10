#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace maths {
    struct Transform {
        glm::vec3 position{0.0f};
        glm::vec3 rotation{0.0f};
    };
} // namespace maths