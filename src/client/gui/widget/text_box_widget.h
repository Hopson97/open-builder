#pragma once

#include "widget.h"

#include <sol/sol.hpp>

namespace gui {

    class TextBoxWidget final : public Widget {
      public:
        TextBoxWidget(TextComponent* textComponent, RectangleComponent* mp_rectangle,
                      TextComponent* label);
        void setPosition(const GuiDimension& position) final override;
        void setSize(const GuiDimension& size) final override;

        void setImage(int image);

        void setText(const std::string& text);
        void setTextSize(unsigned size);
        const std::string& getText() const;

        void setColour(float r, float g, float b) final override;

        void setLabelText(const std::string& text);
        void setPlaceholder(const std::string& text);
        void setMaxLength(int length);

        void handleClick(sf::Mouse::Button button, float mx, float my) final override;
        void handleMouseMove(float mx, float my) final override;
        void allowAllChars(void);
        void limitChars(std::string allowedChars);
        void handleTextEntered(unsigned char code) final override;

        void setOnClick(sol::function function);
        void setOnMouseOver(sol::function function);
        void setOnMouseOff(sol::function function);

        void prepareRender() final override;

        void hideInputText();

      private:
        bool isCharacterAuthorised(unsigned char code);

        TextComponent* mp_text = nullptr;
        RectangleComponent* mp_rectangle = nullptr;
        TextComponent* mp_label = nullptr;

        bool m_isActive = false;
        std::string m_textInput;
        std::string m_displayText;
        std::string m_charsAuthorised;

        std::string m_placeholder = "Enter something...";
        unsigned m_maxLength = 100;

        bool m_isUsingPlaceholder = false;
        bool m_inputHidden = false;

        sol::function m_onMoveOver;
        sol::function m_onMouseOff;
    };
} // namespace gui
