#include "skybox_renderer.h"

#include "../geometry/geometry_factory.h"
#include "camera.h"
#include "gl/gl_errors.h"
#include "mesh.h"
#include <iostream>

namespace client {
    SkyboxRenderer::SkyboxRenderer()
        : m_shader("skybox", "skybox")
    {
        m_shader.use();
        m_locationProjectionMatrix =
            m_shader.getUniformLocation("projectionMatrix");
        m_locationViewMatrix = m_shader.getUniformLocation("viewMatrix");

        float size = 256.0f;
        Mesh mesh;
        mesh.vertices = GeometryFactory::createInvertedCubeVerticies(
            {-size, -size, -size}, {size, size, size});
        mesh.indices = GeometryFactory::createCubeIndices();
        m_skybox.create(mesh);

        m_texture.loadFromFiles({"day_side", "day_side", "day_top",
                                 "day_bottom", "day_side", "day_side"});
    }

    void SkyboxRenderer::render(const Camera &camera)
    {
        // glCheck(glDisable(GL_DEPTH_TEST));
        m_shader.use();
        m_skybox.bind();
        m_texture.bind();

        auto viewMatrix = camera.getViewMatrix();

        viewMatrix[3][0] = 0;
        viewMatrix[3][1] = 0;
        viewMatrix[3][2] = 0;

        m_shader.loadMatrix4(m_locationViewMatrix, viewMatrix);
        m_shader.loadMatrix4(m_locationProjectionMatrix,
                             camera.getProjectionMatrix());

        glCheck(glDrawElements(GL_TRIANGLES, m_skybox.getIndicesCount(),
                               GL_UNSIGNED_INT, nullptr));
        // glCheck(glEnable(GL_DEPTH_TEST));
    }
} // namespace client