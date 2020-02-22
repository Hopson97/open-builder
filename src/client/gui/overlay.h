#pragma once

#include <SFML/Window/Event.hpp>
#include <memory>
#include <vector>

namespace gui {

struct OverlayDefinition;

struct Widget {
    virtual void handleClick(sf::Mouse::Button){};
    virtual void handleMouseMove(sf::Event::MouseMoveEvent){};
    virtual void handleKeyRelease(sf::Keyboard::Key){};
};

struct Overlay {
    Overlay(const OverlayDefinition& overlayDefinition);

    void handleClick(sf::Mouse::Button);
    void handleMouseMove(sf::Event::MouseMoveEvent);
    void handleKeyRelease(sf::Keyboard::Key);

    void addRectangle();

    const OverlayDefinition& definition;

    std::vector<std::unique_ptr<Widget>> widgets;
};

} // namespace gui
