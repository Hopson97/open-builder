#pragma once

#include <glm/gtc/matrix_transform.hpp>

void rotateMatrix(glm::mat4 &matrix, const glm::vec3 &degrees);
void translateMatrix(glm::mat4 &matrix, const glm::vec3 &offset);

glm::vec3 forwardsVector(const glm::vec3 &rotation);
glm::vec3 backwardsVector(const glm::vec3 &rotation);
glm::vec3 leftVector(const glm::vec3 &rotation);
glm::vec3 rightVector(const glm::vec3 &rotation);

class Ray {
  public:
    Ray(const glm::vec3 &startPosition, const glm::vec3 &direction);

    void step();
    float getLength() const;

    const glm::vec3 &getEndpoint() const;
    const glm::vec3 &getLastPoint() const;

  private:
    glm::vec3 m_start;
    glm::vec3 m_previous;
    glm::vec3 m_end;
    glm::vec3 m_direction;
};
