#include "gui_renderer.h"

#include "../gl/primitive.h"
#include "../gui/overlay.h"
#include <glm/gtc/matrix_transform.hpp>

GuiRenderer::GuiRenderer(float viewportWidth, float viewportHeight)
    : m_viewport(viewportWidth, viewportHeight)
    , m_quadVao(makeQuadVertexArray(1.f, 1.f))
{
    glm::mat4 projectionMatrix{1.0f};
    projectionMatrix = glm::ortho(0.0f, viewportWidth, 0.0f, viewportHeight, -1.0f, 1.0f);

    m_shader.bind();
    m_shader.updateProjection(projectionMatrix);

    m_font.init("res/VeraMono-Bold.ttf", 512);
}

int GuiRenderer::getTexture(const std::string& textureName)
{
    assert(m_textureIds.size() == m_textures.size());
    auto itr = m_textureIds.find(textureName);
    if (itr != m_textureIds.end()) {
        return itr->second;
    }
    else {
        int index = m_textures.size();
        gl::Texture2d& texture = m_textures.emplace_back();
        texture.create(textureName, true);
        m_textureIds.emplace(textureName, index);
        return index;
    }
}

void GuiRenderer::render(const gui::Overlay& overlay)
{
    // Render the rectangles of the GUI
    m_shader.bind();
    auto quad = m_quadVao.getDrawable();
    quad.bind();
    for (auto& rect : overlay.rectangleComponents) {
        if (!rect->isHidden()) {
            // TODO Make this line more efficent (maybe?)
            rect->updateBounds(m_viewport);

            auto transform = rect->getRenderTransform(m_viewport);
            m_shader.updateTransform(transform);

            auto& colour = rect->getColour();
            m_shader.updateColour(colour);

            int texture = rect->getTexture();
            if (texture > -1) {
                m_textures.at(texture).bind();
            }
            quad.draw();
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    // Render the text of the GUI
    m_font.bindTexture();
    glCullFace(GL_FRONT);
    glEnable(GL_BLEND);
    auto vp = m_viewport / 100.0f;
    for (auto& text : overlay.textComponents) {
        text->render(m_font, m_shader, vp);
    }
    glCullFace(GL_BACK);
    glDisable(GL_BLEND);
}
