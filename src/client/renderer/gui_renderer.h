#pragma once

#include "../gl/textures.h"
#include "gui_shader.h"
#include <unordered_map>
#include <vector>

namespace gui {
class Overlay;
} // namespace gui

class GuiRenderer final {
  public:
    GuiRenderer(float viewportWidth, float viewportHeight);

    int getTexture(const std::string& textureName);

    void render(const gui::Overlay& overlay);

  private:
    GuiShader2 m_shader;
    glm::vec2 m_viewport;

    std::unordered_map<std::string, int> m_textureIds;
    std::vector<gl::Texture2d> m_textures;
};