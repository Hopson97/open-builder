#pragma once

#include <glm/gtc/matrix_transform.hpp>

void rotateMatrix(glm::mat4 &matrix, const glm::vec3 &degrees);
void translateMatrix(glm::mat4 &matrix, const glm::vec3 &offset);

glm::vec3 forwardsVector(const glm::vec3 &rotation);
glm::vec3 backwardsVector(const glm::vec3 &rotation);
glm::vec3 leftVector(const glm::vec3 &rotation);
glm::vec3 rightVector(const glm::vec3 &rotation);

struct Ray {
    glm::vec3 end{0.0f};
    glm::vec3 lastPosition{0.0f};
    glm::vec3 direction{0.0f};

    void step()
    {
        lastPosition = end;
        end += forwardsVector(direction);
    }
};