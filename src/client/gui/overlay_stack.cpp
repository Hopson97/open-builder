#include "overlay.h"

namespace gui {

void OverlayStack::pushLayer(std::unique_ptr<Overlay> overlay)
{
    overlays.push_back(std::move(overlay));
}

void OverlayStack::popLayer()
{
    m_shouldPop = true;
}

void OverlayStack::removeLayerByName(const std::string& overlayId)
{
    for (auto itr = overlays.begin(); itr != overlays.end();) {
        if ((*itr)->definition.id == overlayId) {
            itr = overlays.erase(itr);
        }
        else {
            itr++;
        }
    }
}

void OverlayStack::handleClick(sf::Mouse::Button button, float mx, float my)
{
    for (auto& layer : overlays) {
        layer->handleClick(button, mx, my);
    }
}

void OverlayStack::handleMouseMove(sf::Event::MouseMoveEvent mouseMoveEvent)
{
    for (auto& layer : overlays) {
        layer->handleMouseMove(mouseMoveEvent);
    }
}

void OverlayStack::handleKeyRelease(sf::Keyboard::Key key)
{
    for (auto& layer : overlays) {
        layer->handleKeyRelease(key);
    }
}

void OverlayStack::update()
{
    if (m_shouldPop) {
        m_shouldPop = false;
        if (overlays.size() > 0) {
            overlays.pop_back();
        }
    }
}
} // namespace gui