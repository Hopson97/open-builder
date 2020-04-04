#include "camera.h"

#include "../client_config.h"

Camera Camera::createCamera(const glm::vec3& position)
{
    Camera camera;
    camera.position = position;

    float width = ClientConfig::get().windowWidth;
    float height = ClientConfig::get().windowHeight;
    camera.m_projection = glm::perspective(3.14f/ 2.0f, width / height, 0.01f, 2000.0f);
    return camera;
}
