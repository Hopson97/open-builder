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