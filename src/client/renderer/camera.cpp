#include "camera.h"

#include "../util/maths.h"
#include <common/launch_config.h>

namespace client {
    Camera::Camera()
    {
        m_projectionMatrix = glm::perspective(glm::radians(90.0f),
                                              1280.0f / 720.0f, 0.01f, 128.0f);

        updateMatrices();
    }

    Camera::Camera(const LaunchConfig &config)
    {
        m_projectionMatrix = makeProjectionMatrix(config);

        updateMatrices();
    }

    void Camera::reset(const maths::Transform &transform)
    {
        m_transform = transform;
        m_transform.position.y += 1.7;
        updateMatrices();
    }

    void Camera::updateMatrices()
    {
        m_viewMatrix = makeViewMatrix(m_transform);
        m_projectionViewMatrix = m_projectionMatrix * m_viewMatrix;
    }

    void Camera::setRotation(const glm::vec3 &rotation)
    {
        m_transform.rotation = rotation;
        updateMatrices();
    }

    void Camera::setPosition(const glm::vec3 &position)
    {
        m_transform.position = position;
        updateMatrices();
    }

    const glm::vec3 &Camera::getPosition() const
    {
        return m_transform.position;
    }

    const glm::mat4 &Camera::getViewMatrix() const
    {
        return m_viewMatrix;
    }

    const glm::mat4 &Camera::getProjectionMatrix() const
    {
        return m_projectionMatrix;
    }

    const glm::mat4 &Camera::getProjectionViewMatrix() const
    {
        return m_projectionViewMatrix;
    }
} // namespace client