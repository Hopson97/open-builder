#pragma once

#include <SFML/Window/Event.hpp>

namespace gui {

struct OverlayDefinition;

struct Overlay {
    Overlay(const OverlayDefinition& overlayDefinition);

    void handleClick(sf::Mouse::Button);
    void handleMouseMove(sf::Event::MouseMoveEvent);
    void handleKeyRelease(sf::Keyboard::Key);

    const OverlayDefinition& definition;
};

} // namespace gui
