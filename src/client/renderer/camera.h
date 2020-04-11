#pragma once

#include <common/maths.h>

#include <common/world/entity_state.h>

class Camera {
  public:
    static Camera createCamera();
    void update(const EntityState& entity);

    const ViewFrustum& getFrustum() const;
    const glm::mat4& getProjectionView() const;
    const glm::vec3& getPosition() const;

  private:
    glm::mat4 m_projectionMatrix{1.0f};
    glm::mat4 m_projectionViewMatrix{1.0f};
    glm::vec3 m_position{1.0f};
    glm::vec3 m_rotation{1.0f};

    ViewFrustum m_frustum;
};
