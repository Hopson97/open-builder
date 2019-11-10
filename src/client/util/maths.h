#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct LaunchConfig;
namespace maths {
    struct Transform;
}

namespace client {
    glm::mat4 makeModelMatrix(const maths::Transform &transform,
                              const glm::vec3 &offset = {0, 0, 0});
    glm::mat4 makeViewMatrix(const maths::Transform &transform);
    glm::mat4 makeProjectionMatrix(const LaunchConfig &config);

} // namespace client
