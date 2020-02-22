#include "gui_renderer.h"

#include "../gl/primitive.h"
#include <glm/gtc/matrix_transform.hpp>

GuiRenderer::GuiRenderer(float viewportWidth, float viewportHeight)
    : m_viewport(viewportWidth, viewportHeight)
{
    glm::mat4 projectionMatrix{1.0f};
    projectionMatrix = glm::ortho(0.0f, viewportWidth, 0.0f, viewportHeight, -1.0f, 1.0f);

    m_shader.bind();
    m_shader.updateProjection(projectionMatrix);
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
    // Render rectangles first

    // Render text
}
