#include "entity.h"

namespace server {
    void Entity::moveForwards()
    {
        velocity.x += -glm::cos(glm::radians(rotation.y + 90)) * speed;
        velocity.z += -glm::sin(glm::radians(rotation.y + 90)) * speed;
    }

    void Entity::moveLeft()
    {
        velocity.x += -glm::cos(glm::radians(rotation.y)) * speed;
        velocity.z += -glm::sin(glm::radians(rotation.y)) * speed;
    }

    void Entity::moveRight()
    {
        velocity.x += glm::cos(glm::radians(rotation.y)) * speed;
        velocity.z += glm::sin(glm::radians(rotation.y)) * speed;
    }

    void Entity::moveBackwards()
    {
        velocity.x += glm::cos(glm::radians(rotation.y + 90)) * speed;
        velocity.z += glm::sin(glm::radians(rotation.y + 90)) * speed;
    }
} // namespace server
