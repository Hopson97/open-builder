#pragma once

#include "../gl/vertex_array.h"
#include "gui_dimension.h"
#include <string>

class GuiShader;
class Font;

class GuiText {
  public:
    GuiText(Font& font);

    void setFont(const Font& font);

    void setPosition(const GuiDimension& position);
    void setFontSize(float size);
    void setText(const std::string& text);

    void render(GuiShader& shader, const glm::vec2& viewport);

  private:
    void updateGeometry();

    gl::VertexArray m_textQuads;
    std::string m_text;
    GuiDimension m_position;
    float m_fontSize = 0;

    const Font* mp_font = nullptr;

    bool m_isGeometryUpdateNeeded = true;
};