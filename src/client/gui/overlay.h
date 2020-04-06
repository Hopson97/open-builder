#pragma once

#include "component/rectangle_component.h"
#include "component/text_component.h"
#include "widget/widget.h"
#include <memory>
#include <sol/sol.hpp>
#include <vector>

namespace gui {

    class ImageWidget;
    class LabelWidget;
    class ButtonWidget;
    class CheckBoxWidget;
    class CenteredLabelWidget;
    class TextBoxWidget;
    /**
     * @brief Defines how a GUI should be created.
     * This is created in the Client Lua code
     */
    struct OverlayDefinition final {
        std::string id;

        sol::function create;
    };

    /**
     * @brief Holds the components that make up some GUI overlay, eg HUD, menus etc
     */
    class Overlay final {
      public:
        Overlay(const OverlayDefinition& overlayDefinition);

        void handleClick(sf::Mouse::Button button, float mx, float my);
        void handleMouseMove(sf::Event::MouseMoveEvent);
        void handleKeyRelease(sf::Keyboard::Key);
        void handleTextEntered(unsigned char keycode);

        // These widget adding function return non-owning pointers
        // They are mostly called via Lua code, which is why it is pointers
        ImageWidget* addImage();
        LabelWidget* addLabel();
        CenteredLabelWidget* addCenteredLabel();
        CheckBoxWidget* addCheckBox();
        ButtonWidget* addButton();
        TextBoxWidget* addTextBox();

        void prepareWidgetsForRender();

        void hide();
        void show();
        bool isHidden() const;

        // The overlay defintion that created this overlay
        const OverlayDefinition& definition;

        // Stored as pointers to allow them to be stored/used by the Lua code
        std::vector<std::unique_ptr<RectangleComponent>> rectangleComponents;
        std::vector<std::unique_ptr<TextComponent>> textComponents;

      private:
        std::vector<std::unique_ptr<Widget>> m_widgets;
        bool m_isHidden = false;
    };

} // namespace gui
