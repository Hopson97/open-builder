#include "entity.h"

#include <iostream>

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

    void Entity::tick()
    {
        if (glm::distance(position, target) > 2.0f) {
            moveRight();
        }
        else {
            target.x = rand() % 32;
            target.z = rand() % 32;
            float dx = target.x - position.x;
            float dz = target.z - position.z;
            rotation.y = glm::degrees(std::atan2(dz, dx));
        }
    }
} // namespace server
