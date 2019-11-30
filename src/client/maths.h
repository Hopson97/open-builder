#pragma once

#include <glm/gtc/matrix_transform.hpp>

void rotateMatrix(glm::mat4 *matrix, const glm::vec3 &degrees);
void translateMatrix(glm::mat4 *matrix, const glm::vec3 &offset);