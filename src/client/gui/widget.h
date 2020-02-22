#pragma once

#include "component.h"
#include <SFML/Window/Event.hpp>

namespace gui {
struct Widget {
    virtual void handleClick(sf::Mouse::Button){};
    virtual void handleMouseMove(sf::Event::MouseMoveEvent){};
    virtual void handleKeyRelease(sf::Keyboard::Key){};

    virtual void setPosition(const GuiDimension& position) = 0;
    virtual void setSize(const GuiDimension& size) = 0;
};


} // namespace gui
