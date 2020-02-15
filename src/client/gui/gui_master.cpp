#include "gui_master.h"

#include "../gl/primitive.h"
#include <glm/gtc/matrix_transform.hpp>

GuiMaster::GuiMaster(float viewportWidth, float viewportHeight)
    : m_quadVao(makeQuadVertexArray(1.f, 1.f))
    , m_viewport(viewportWidth, viewportHeight)
{
    m_projection = glm::ortho(0.0f, viewportWidth, 0.0f, viewportHeight, -1.0f, 1.0f);

    m_shader.bind();
    m_shader.updateProjection(m_projection);
}

GuiContainer* GuiMaster::addGui()
{
    return &m_containers.emplace_back();
}

void GuiMaster::render()
{
    glm::vec2 viewport = m_viewport / 100.0f;
    auto quad = m_quadVao.getDrawable();
    quad.bind();
    m_shader.bind();
    for (auto& container : m_containers) {
        container.render(m_shader, viewport, quad, m_textures);
    }
}

int GuiMaster::getTexture(const std::string& textureName)
{
    assert(m_textureIds.size() == m_textures.size());
    auto itr = m_textureIds.find(textureName);
    if (itr != m_textureIds.end()) {
        return itr->second;
    }
    else {
        int index = m_textures.size();
        gl::Texture2d& texture = m_textures.emplace_back();
        texture.create(textureName);
        m_textureIds.emplace(textureName, index);
        return index;
    }
}

int GuiMaster::guiCount() const
{
    return m_containers.size();
}

int GuiMaster::textureCount() const
{
    return m_containers.size();
}