#pragma once

#include <common/util/maths.h>

#include "gl/shader.h"
#include "model/textured_model.h"
#include "skybox_renderer.h"
#include "chunk_renderer.h"

#include "../world/chunk/mesh/chunk_mesh.h"

#include <unordered_map>

namespace client {
    class Camera;
    class Scene;
    class TexturedModel;
    class Entity;

    class Renderer final {
      public:
        Renderer();
        ~Renderer() = default;

        Renderer(Renderer &&) = delete;
        Renderer &operator=(Renderer &&) = delete;

        Renderer(const Renderer &) = delete;
        Renderer &operator=(const Renderer &) = delete;

        void process(const Entity &entity);
        void process(const ChunkMesh& mesh);

        void render(const Camera &camera);

        bool doesDrawSky = true;

      private:
        std::unordered_map<const TexturedModel *, std::vector<maths::Transform>>
            m_entityBatches;

        SkyboxRenderer m_skyboxRenderer;
        ChunkRenderer m_chunkRenderer;

        Shader m_entityShader;
        GLuint m_locationProjectionViewMatrix;
        GLuint m_locationModelMatrix;
        GLuint m_locationColour;
        GLuint m_locationLightPosition;

        ChunkMesh m_chunkMesh;
    };
} // namespace client
