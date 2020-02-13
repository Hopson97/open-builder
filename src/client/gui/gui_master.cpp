#include "gui_master.h"

GuiMaster::GuiMaster(int viewportWidth, int viewportHeight)
{

}

void GuiMaster::addGui()
{
    
}

void GuiMaster::render()
{

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