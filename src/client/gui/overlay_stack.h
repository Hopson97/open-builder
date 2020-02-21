#pragma once

#include "overlay.h"
#include <memory>
#include <vector>

namespace gui {

struct OverlayStack {
    void pushLayer(std::unique_ptr<Overlay>);
    void popLayer();
    void removeLayerByName(const std::string& overlayId);

    void handleClick(sf::Mouse::Button);
    void handleMouseMove(sf::Event::MouseMoveEvent);
    void handleKeyRelease(sf::Keyboard::Key);

    std::vector<std::unique_ptr<Overlay>> m_overlayStack;
};
} // namespace gui