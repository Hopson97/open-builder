#pragma once

#include "component.h"
#include <SFML/System/Vector2.hpp>

namespace gl {
    class Font;
}
class GuiShader;

namespace gui {

    class TextComponent final : public Component {
      public:
        void setPosition(const GuiDimension& position) final override;
        void setSize(const GuiDimension& size) final override;

        void setFontSize(unsigned size);
        void setText(const std::string& text);

        void render(gl::Font& font, GuiShader& shader);

        const sf::Vector2f& getSize() const;
        const GuiDimension& getPosition() const;

      private:
        void updateGeometry(gl::Font& font);

        gl::VertexArray m_textQuads;
        std::string m_text;
        GuiDimension m_position;
        unsigned m_fontSize = 0;

        sf::Vector2f m_size;

        bool m_isGeometryUpdateNeeded = true;
    };
} // namespace gui
