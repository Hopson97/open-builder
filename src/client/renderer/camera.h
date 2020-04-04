#pragma once

#include "../maths.h"

#include <common/world/entity_state.h>

struct Camera {
  public:
    static Camera createCamera(const EntityState& entityToFollow);

    void follow(const EntityState& state);
    void update();
    const glm::mat4& getProjectionView();

  private:
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_projectionViewMatrix;

    ViewFrustum m_frustum;

    const EntityState* mp_entityFollowing;
};