#include "entity.h"

#include <cmath>

namespace server {
    Entity::Entity()
    {
          
    }

    void Entity::moveForwards()
    {
        velocity.x += -glm::cos(glm::radians(transform.rotation.y + 90));
        velocity.z += -glm::sin(glm::radians(transform.rotation.y + 90));
    }

    void Entity::moveLeft()
    {
        velocity.x += -glm::cos(glm::radians(transform.rotation.y));
        velocity.z += -glm::sin(glm::radians(transform.rotation.y));
    }

    void Entity::moveRight()
    {
        velocity.x += glm::cos(glm::radians(transform.rotation.y));
        velocity.z += glm::sin(glm::radians(transform.rotation.y));
    }

    void Entity::moveBackwards()
    {
        velocity.x += glm::cos(glm::radians(transform.rotation.y + 90));
        velocity.z += glm::sin(glm::radians(transform.rotation.y + 90));
    }
} // namespace server
