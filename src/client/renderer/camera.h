#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <common/util/maths.h>

struct LaunchConfig;

namespace client {
    class Camera final {
      public:
        Camera();
        Camera(const LaunchConfig &config);

        void reset(const maths::Transform &transform);

        const glm::mat4 &getViewMatrix() const;
        const glm::mat4 &getProjectionMatrix() const;
        const glm::mat4 &getProjectionViewMatrix() const;

        void setPosition(const glm::vec3 &position);
        void setRotation(const glm::vec3 &position);

        const glm::vec3 &getPosition() const;

      private:
        void updateMatrices();

        glm::mat4 m_projectionMatrix{1.0f};
        glm::mat4 m_orthoMatrix{1.0f};

        glm::mat4 m_viewMatrix{1.0f};
        glm::mat4 m_projectionViewMatrix{1.0f};

        maths::Transform m_transform;
    };
} // namespace client
