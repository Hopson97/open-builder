#include "overlay.h"

namespace gui {

void OverlayStack::pushLayer(std::unique_ptr<Overlay> overlay)
{
    overlays.push_back(std::move(overlay));
}

void OverlayStack::popLayer()
{
    overlays.pop_back();
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

void OverlayStack::handleClick(sf::Mouse::Button button)
{
    for (auto& layer : overlays) {
        layer->handleClick(button);
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

} // namespace gui