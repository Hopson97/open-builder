#pragma once

#include <glm/gtc/matrix_transform.hpp>

void rotateMatrix(glm::mat4 *matrix, const glm::vec3 &degrees);
void translateMatrix(glm::mat4 *matrix, const glm::vec3 &offset);

glm::vec3 forwardsVector(const glm::vec3 &rotation);
glm::vec3 backwardsVector(const glm::vec3 &rotation);
glm::vec3 leftVector(const glm::vec3 &rotation);
glm::vec3 rightVector(const glm::vec3 &rotation);