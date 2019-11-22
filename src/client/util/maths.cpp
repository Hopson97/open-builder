#include "maths.h"

#include <common/util/maths.h>

#include "../client_config.h"
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

    glm::mat4 makeProjectionMatrix(const Config &config)
    {
        float aspect = config.windowWidth /
                       config.windowHeight;
        float fov = glm::radians(static_cast<float>(config.fov));

        return glm::perspective(fov, aspect, 0.01f, 2048.0f);
    }
} // namespace client
// clang-format on