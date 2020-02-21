#include "overlay_stack.h"

#include "overlay_factory.h"

namespace gui {

void OverlayStack::pushLayer(std::unique_ptr<Overlay> overlay)
{
    m_overlayStack.push_back(std::move(overlay));
}

void OverlayStack::popLayer()
{
    m_overlayStack.pop_back();
}

void OverlayStack::removeLayerByName(const std::string& overlayId)
{
    for (auto itr = m_overlayStack.begin(); itr != m_overlayStack.end();) {
        if ((*itr)->definition.id == overlayId) {
            itr = m_overlayStack.erase(itr);
        }
        else {
            itr++;
        }
    }
}

void OverlayStack::handleClick(sf::Mouse::Button button)
{
    for (auto& layer : m_overlayStack) {
        layer->handleClick(button);
    }
}

void OverlayStack::handleMouseMove(sf::Event::MouseMoveEvent mouseMoveEvent)
{
    for (auto& layer : m_overlayStack) {
        layer->handleMouseMove(mouseMoveEvent);
    }
}

void OverlayStack::handleKeyRelease(sf::Keyboard::Key key)
{
    for (auto& layer : m_overlayStack) {
        layer->handleKeyRelease(key);
    }
}

} // namespace gui