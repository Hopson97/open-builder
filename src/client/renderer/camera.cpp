#include "camera.h"

#include "../client_config.h"
#include <cassert>

Camera Camera::createCamera(const EntityState& entityToFollow)
{
    Camera camera;

    camera.mp_entityFollowing = &entityToFollow;

    float width = ClientConfig::get().windowWidth;
    float height = ClientConfig::get().windowHeight;
    camera.m_projectionMatrix =
        glm::perspective(3.14f / 2.0f, width / height, 0.01f, 2000.0f);
    return camera;
}

void Camera::follow(const EntityState& state)
{
    mp_entityFollowing - &state;
}

void Camera::update()
{
    assert(mp_entityFollowing);
    m_frustum.update(m_projectionViewMatrix);

    auto& position = mp_entityFollowing->position;
    auto& rotation = mp_entityFollowing->rotation;

    m_projectionViewMatrix = createProjectionViewMatrix(position, rotation, m_projectionMatrix);
}

const glm::mat4& Camera::getProjectionView()
{
    return m_projectionViewMatrix;
}
