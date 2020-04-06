#pragma once

#include <SFML/Window/Event.hpp>
#include <vector>

namespace gui {
    struct GuiDimension;
    class RectangleComponent;
    class TextComponent;
    class Component;

    /**
     * @brief Base class for different widgets that can makeup a GUI overlay
     */
    struct Widget {
        virtual ~Widget() = default;
        virtual void handleClick(sf::Mouse::Button, float, float){};
        virtual void handleMouseMove(float, float){};
        virtual void handleKeyRelease(sf::Keyboard::Key){};
        virtual void handleTextEntered(unsigned char)
        {
        }
        virtual void prepareRender(){};
        virtual void setColour(float r, float g, float b);

        virtual void setPosition(const GuiDimension& position) = 0;
        virtual void setSize(const GuiDimension& size) = 0;

        void hide();
        void show();

        std::vector<Component*> componentList;
    };

} // namespace gui
