#include "maths.h"

#include <common/launch_config.h>
#include <common/util/maths.h>

// clang-format off
namespace client {

    glm::mat4 makeModelMatrix(const maths::Transform &transform, [[maybe_unused]]const glm::vec3& offset)
    {
        glm::mat4 matrix(1.0f);

        matrix = glm::translate(matrix, transform.position);

        //matrix = glm::translate(matrix, offset);

        matrix = glm::rotate(matrix, glm::radians(transform.rotation.x), {1, 0, 0});
        matrix = glm::rotate(matrix, glm::radians(transform.rotation.y), {0, 1, 0});
        matrix = glm::rotate(matrix, glm::radians(transform.rotation.z), {0, 0, 1});

       // matrix = glm::translate(matrix, -offset);

        return matrix;
    }

    glm::mat4 makeViewMatrix(const maths::Transform &transform)
    {
        glm::mat4 matrix(1.0f);

        matrix = glm::rotate(matrix, glm::radians(transform.rotation.x), {1, 0, 0});
        matrix = glm::rotate(matrix, glm::radians(transform.rotation.y), {0, 1, 0});
        matrix = glm::rotate(matrix, glm::radians(transform.rotation.z), {0, 0, 1});

        matrix = glm::translate(matrix, -transform.position);

        return matrix;
    }

    glm::mat4 makeProjectionMatrix(const LaunchConfig &config)
    {
        float aspect = config.graphicOptions.windowWidth /
                       config.graphicOptions.windowHeight;
        float fov = glm::radians(static_cast<float>(config.graphicOptions.fov));

        return glm::perspective(fov, aspect, 0.01f, 256.0f);
    }
} // namespace client
// clang-format on