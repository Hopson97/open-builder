#include "gui_master.h"

#include "../gl/primitive.h"

GuiMaster::GuiMaster(int viewportWidth, int viewportHeight)
    : m_quadVao(makeQuadVertexArray(1.f, 1.f))
{

}

void GuiMaster::addGui(GuiContainer& container)
{
    m_containers.push_back(&container);
}

void GuiMaster::render()
{
    m_quadVao.bind();
    for (auto container : m_containers) {
        container->render();
    }
}

int GuiMaster::getTexture(const std::string& textureName)
{
    auto itr = m_textureIds.find(textureName);
    if (itr == m_textureIds.end()) {
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