#pragma once

#include "../gl/vertex_array.h"
#include "gui_dimension.h"
#include <string>

class GuiShader;

namespace gl {
class Font;
}

class GuiText {
  public:
    GuiText(const gl::Font& font);

    void setFont(const gl::Font& font);

    void setPosition(const GuiDimension& position);
    void setFontSize(float size);
    void setText(const std::string& text);

    void render(GuiShader& shader, const glm::vec2& viewport);

    void hide();
    void show();

  private:
    void updateGeometry();

    gl::VertexArray m_textQuads;
    std::string m_text;
    GuiDimension m_position;
    float m_fontSize = 0;

    const gl::Font* mp_font = nullptr;

    bool m_isGeometryUpdateNeeded = true;
    bool m_isHidden = false;
};