#pragma once

#include "../gl/font.h"
#include "../gl/textures.h"
#include "../gl/vertex_array.h"
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
    GuiShader m_shader;
    glm::vec2 m_viewport;

    std::unordered_map<std::string, int> m_textureIds;
    std::vector<gl::Texture2d> m_textures;

    gl::VertexArray m_quadVao;
    gl::Font m_font;
};