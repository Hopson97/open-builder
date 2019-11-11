#pragma once

#include "texture/cube_texture.h"
#include "gl/shader.h"
#include "model/vertex_array.h"

namespace client {
    class Camera;
    class SkyboxRenderer final {
      public:
        SkyboxRenderer();

        void render(const Camera &camera);

      private:
        CubeTexture m_texture;
        Shader m_shader;
        VertexArray m_skybox;

        GLuint m_locationProjectionMatrix;
        GLuint m_locationViewMatrix;
    };
} // namespace client
