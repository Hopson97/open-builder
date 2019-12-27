#include "maths.h"

void rotateMatrix(glm::mat4 &matrix, const glm::vec3 &degrees)
{
    matrix = glm::rotate(matrix, glm::radians(degrees.x), {1, 0, 0});
    matrix = glm::rotate(matrix, glm::radians(degrees.y), {0, 1, 0});
    matrix = glm::rotate(matrix, glm::radians(degrees.z), {0, 0, 1});
}

void translateMatrix(glm::mat4 &matrix, const glm::vec3 &offset)
{
    matrix = glm::translate(matrix, offset);
}

glm::vec3 forwardsVector(const glm::vec3 &rotation)
{
    float yaw = glm::radians(rotation.y + 90);
    float pitch = glm::radians(rotation.x);

    return {-(glm::cos(yaw) * glm::cos(pitch)), -glm::sin(pitch),
            -(glm::cos(pitch) * glm::sin(yaw))};
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

Ray::Ray(const glm::vec3 &startPosition, const glm::vec3 &direction)
    : m_start(startPosition)
    , m_previous(startPosition)
    , m_end(startPosition)
    , m_direction(direction)

{
}

void Ray::step()
{
    m_previous = m_end;
    m_end += forwardsVector(m_direction) / 4.0f;
}

float Ray::getLength() const
{
    return glm::length(m_end - m_start);
}

const glm::vec3 &Ray::getEndpoint() const
{
    return m_end;
}

const glm::vec3 &Ray::getLastPoint() const
{
    return m_previous;
}