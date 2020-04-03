#pragma once

#include "widget.h"

#include <sol/sol.hpp>

namespace gui {

    class ButtonWidget final : public Widget {
      public:
        ButtonWidget(TextComponent* textComponent, RectangleComponent* mp_rectangle);
        void setPosition(const GuiDimension& position) final override;
        void setSize(const GuiDimension& size) final override;

        void setImage(int image);

        void setText(const std::string& text);
        void setTextSize(unsigned size);

        void setColour(float r, float g, float b) final override;

        void handleClick(sf::Mouse::Button button, float mx, float my) final override;
        void handleMouseMove(float mx, float my) final override;

        void setOnClick(std::function<void(void)> function);
        void setOnClickLua(sol::function function);
        void setOnMouseOver(sol::function function);
        void setOnMouseOff(sol::function function);

        void prepareRender() final override;

      private:
        TextComponent* mp_text = nullptr;
        RectangleComponent* mp_rectangle = nullptr;

        std::function<void(void)> m_onClick;
        sol::function m_onClickLua;
        sol::function m_onMoveOver;
        sol::function m_onMouseOff;
    };
} // namespace gui
