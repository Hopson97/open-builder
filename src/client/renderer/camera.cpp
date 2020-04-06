#include "camera.h"

#include "../client_config.h"
#include <cassert>

Camera Camera::createCamera()
{
    Camera camera;
    float width = ClientConfig::get().windowWidth;
    float height = ClientConfig::get().windowHeight;
    camera.m_projectionMatrix =
        glm::perspective(3.14f / 2.0f, width / height, 0.01f, 2000.0f);
    return camera;
}

void Camera::update(const EntityState& entity)
{
    m_frustum.update(m_projectionViewMatrix);

    m_position = entity.position;
    m_rotation = entity.rotation;

    m_projectionViewMatrix =
        createProjectionViewMatrix(m_position, m_rotation, m_projectionMatrix);
}

const ViewFrustum& Camera::getFrustum() const
{
    return m_frustum;
}

const glm::mat4& Camera::getProjectionView() const
{
    return m_projectionViewMatrix;
}

const glm::vec3& Camera::getPosition() const
{
    return m_position;
}
