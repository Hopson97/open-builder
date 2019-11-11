#include "entity.h"

namespace server {
    void Entity::moveForwards()
    {
        velocity.x += -glm::cos(glm::radians(rotation.y + 90));
        velocity.z += -glm::sin(glm::radians(rotation.y + 90));
    }

    void Entity::moveLeft()
    {
        velocity.x += -glm::cos(glm::radians(rotation.y));
        velocity.z += -glm::sin(glm::radians(rotation.y));
    }

    void Entity::moveRight()
    {
        velocity.x += glm::cos(glm::radians(rotation.y));
        velocity.z += glm::sin(glm::radians(rotation.y));
    }

    void Entity::moveBackwards()
    {
        velocity.x += glm::cos(glm::radians(rotation.y + 90));
        velocity.z += glm::sin(glm::radians(rotation.y + 90));
    }
} // namespace server
