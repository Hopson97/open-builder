#include "renderer.h"

#include "camera.h"
#include "gl/gl_errors.h"
#include <iostream>

#include "../geometry/geometry_factory.h"
#include "../util/maths.h"

#include "../world/entity.h"

#include "../world/chunk/chunk_section.h"
#include "../world/chunk/mesh/chunk_mesh_builder.h"

namespace client {
    void drawElements(GLsizei count)
    {
        glCheck(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
    }

    Renderer::Renderer()
        : m_entityShader("static", "static")
    {
        m_entityShader.use();

        m_locationProjectionViewMatrix =
            m_entityShader.getUniformLocation("projectionViewMatrix");
        m_locationModelMatrix =
            m_entityShader.getUniformLocation("modelMatrix");
        m_locationColour = m_entityShader.getUniformLocation("colour");
        m_locationLightPosition =
            m_entityShader.getUniformLocation("lightPosition");

        glCheck(glEnable(GL_DEPTH_TEST));
        //glCheck(glEnable(GL_CULL_FACE));
        //glCheck(glCullFace(GL_BACK));
    }

    void Renderer::process(const ChunkMesh &mesh)
    {
        m_chunkRenderer.process(mesh);
    }

    void Renderer::process(const Entity &entity)
    {
        if (m_entityBatches.find(entity.model) != m_entityBatches.cend()) {
            m_entityBatches[entity.model].push_back(entity.transform);
        }
        else {
            std::vector<maths::Transform> entities;
            entities.push_back(entity.transform);
            m_entityBatches.emplace(entity.model, entities);
        }
    }

    void Renderer::render(const Camera &camera)
    {
        // Skybox is drawn without depth testing
        // So it must be rendered before anything else
        m_skyboxRenderer.render(camera);

        m_entityShader.use();
        m_entityShader.loadVector3(m_locationColour, {1, 1, 1});
        m_entityShader.loadMatrix4(m_locationProjectionViewMatrix,
                                   camera.getProjectionViewMatrix());
        m_entityShader.loadVector3(m_locationLightPosition,
                                   {camera.getPosition().x,
                                    camera.getPosition().y + 0.5,
                                    camera.getPosition().z});

        for (auto &batch : m_entityBatches) {
            batch.first->bind();
            for (auto entity : batch.second) {
                auto mat = makeModelMatrix(entity, {0.5f, 0.f, 0.5f});
                m_entityShader.loadMatrix4(m_locationModelMatrix, mat);
                drawElements(batch.first->getIndicesCount());
            }
        }
        m_entityBatches.clear();
        m_chunkRenderer.render(camera);
    }
} // namespace client
