#pragma once

#include "widget.h"

#include <sol/sol.hpp>

namespace gui {

    class CheckBoxWidget final : public Widget {
      public:
        CheckBoxWidget(RectangleComponent* mp_rectangle, TextComponent* label);
        void setPosition(const GuiDimension& position) final override;
        void setSize(const GuiDimension& size) final override;

        void setColour(float r, float g, float b) final override;
        void setImage(int Image);
        void setUncheckedImage(int Image);
        void setCheckedImage(int Image);

        void setText(const std::string& text);
        void setTextSize(unsigned size);

        void setChecked(bool check);

        void handleClick(sf::Mouse::Button, float, float) final override;
        void handleMouseMove(float, float) final override;

        void onClick();
        void setOnMouseOver(sol::function function);
        void setOnMouseOff(sol::function function);

        void prepareRender() final override;

        bool getChecked();

      private:
        int checkedTexture;
        int uncheckedTexture;

        RectangleComponent* mp_rectangle = nullptr;
        TextComponent* mp_label = nullptr;
        bool m_checked = false;

        sol::function m_onMoveOver;
        sol::function m_onMouseOff;
    };

} // namespace gui
