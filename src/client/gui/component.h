#pragma once

#include "../gl/vertex_array.h"
#include <SFML/Graphics/Rect.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

namespace gl {
class Font;
}
class GuiShader2;

namespace gui {

struct GuiDimension final {
    glm::vec2 scale{0.0f};
    glm::vec2 offset{0.0f};

    GuiDimension() = default;

    GuiDimension(float xScale, float xOffset, float yScale, float yOffset)
        : scale{xScale, yScale}
        , offset{xOffset, yOffset}
    {
    }

    glm::vec2 apply(const glm::vec2& viewport) const
    {
        glm::vec2 vector;
        vector.x = scale.x * viewport.x * 100.0f;
        vector.y = scale.y * viewport.y * 100.0f;
        return vector + offset;
    }
};

struct RectangleComponent final {
  public:
    glm::mat4 getRenderTransform(const glm::vec2& viewport) const;

    // Setters rather than "public" as it makes it a lot easier for a clean lua api
    void setPosition(const GuiDimension& position);
    void setSize(const GuiDimension& size);
    void setTexture(int texture);
    void setColour(float r, float g, float b);

    int getTexture() const;
    const glm::vec3& getColour() const;

    void hide();
    void show();
    bool isHidden() const;

  private:
    void updateBounds();

    sf::FloatRect m_bounds;
    GuiDimension m_position;
    GuiDimension m_size;
    glm::vec3 m_colour{1.0f};

    int m_texture = -1;

    bool m_isHidden = false;
};

class TextComponent {
  public:
    TextComponent(const gl::Font& font);

    void setFont(const gl::Font& font);

    void setPosition(const GuiDimension& position);
    void setFontSize(float size);
    void setText(const std::string& text);

    void render(GuiShader2& shader, const glm::vec2& viewport);

    void hide();
    void show();

  private:
    void updateGeometry();

    ::gl::VertexArray m_textQuads;
    std::string m_text;
    GuiDimension m_position;
    float m_fontSize = 0;

    const gl::Font* mp_font = nullptr;

    bool m_isGeometryUpdateNeeded = true;
    bool m_isHidden = false;
};

} // namespace gui