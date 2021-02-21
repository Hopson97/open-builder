#include "Maths.h"

glm::mat4 createModelMatrix(const glm::vec3& pos, const glm::vec3& rot)
{
    glm::mat4 matrix{1.0f};

    matrix = glm::translate(matrix, pos);

    matrix = glm::rotate(matrix, glm::radians(rot.x), {1, 0, 0});
    matrix = glm::rotate(matrix, glm::radians(rot.y), {0, 1, 0});
    matrix = glm::rotate(matrix, glm::radians(rot.z), {0, 0, 1});
    return matrix;
}

glm::mat4 createViewMartix(const glm::vec3& pos, const glm::vec3& rot)
{
    glm::mat4 matrix{1.0f};

    matrix = glm::rotate(matrix, glm::radians(rot.x), {1, 0, 0});
    matrix = glm::rotate(matrix, glm::radians(rot.y), {0, 1, 0});
    matrix = glm::rotate(matrix, glm::radians(rot.z), {0, 0, 1});
    matrix = glm::translate(matrix, -pos);

    return matrix;
}

glm::mat4 createProjectionMatrix(float aspectRatio, float fov)
{
    return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1024.0f);
}

glm::vec3 forwardsVector(const glm::vec3& rotation)
{
    float yaw = glm::radians(rotation.y + 90);
    float pitch = glm::radians(rotation.x);
    float x = glm::cos(yaw) * glm::cos(pitch);
    float y = glm::sin(pitch);
    float z = glm::cos(pitch) * glm::sin(yaw);

    return {-x, -y, -z};
}

glm::vec3 backwardsVector(const glm::vec3& rotation)
{
    return -forwardsVector(rotation);
}

glm::vec3 leftVector(const glm::vec3& rotation)
{
    float yaw = glm::radians(rotation.y);
    float x = glm::cos(yaw);
    float y = 0;
    float z = glm::sin(yaw);

    return {-x, -y, -z};
}

glm::vec3 rightVector(const glm::vec3& rotation)
{
    return -leftVector(rotation);
}