#include "maths.h"

void rotateMatrix(glm::mat4 *matrix, const glm::vec3 &degrees)
{
    *matrix = glm::rotate(*matrix, glm::radians(degrees.x), {1, 0, 0});
    *matrix = glm::rotate(*matrix, glm::radians(degrees.y), {0, 1, 0});
    *matrix = glm::rotate(*matrix, glm::radians(degrees.z), {0, 0, 1});
}

void translateMatrix(glm::mat4 *matrix, const glm::vec3 &offset)
{
    *matrix = glm::translate(*matrix, offset);
}

glm::vec3 forwardsVector(const glm::vec3 &rotation)
{
    float yRotation = glm::radians(rotation.y + 90);
    float xRotation = glm::radians(rotation.x);
    return {-glm::cos(yRotation), -glm::tan(xRotation), -glm::sin(yRotation)};
}

glm::vec3 backwardsVector(const glm::vec3 &rotation)
{
    return -forwardsVector(rotation);
}

glm::vec3 leftVector(const glm::vec3 &rotation)
{
    float yRotation = glm::radians(rotation.y);
    return {-glm::cos(yRotation), 0, -glm::sin(yRotation)};
}

glm::vec3 rightVector(const glm::vec3 &rotation)
{
    return -leftVector(rotation);
}