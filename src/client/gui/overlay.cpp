#include "overlay.h"

namespace gui {
Overlay::Overlay(const OverlayDefinition& overlayDefinition)
    : definition(overlayDefinition)
{
}

void Overlay::handleClick(sf::Mouse::Button)
{
}

void Overlay::handleMouseMove(sf::Event::MouseMoveEvent)
{
}

void Overlay::handleKeyRelease(sf::Keyboard::Key)
{
}
} // namespace gui
