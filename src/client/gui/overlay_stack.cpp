#include "overlay.h"

namespace gui {

OverlayStack::OverlayStack(float winWidth, float winHeight)
    : m_windowWidth(winWidth)
    , m_windowHeight(winHeight)
{
}

void OverlayStack::pushLayer(std::unique_ptr<Overlay> overlay)
{
    Action action;
    action.type = ActionType::PushLayer;
    action.overlay = std::move(overlay);
    m_pendingActions.push(std::move(action));
}

void OverlayStack::popLayer()
{
    Action action;
    action.type = ActionType::PopLayer;
    m_pendingActions.push(std::move(action));
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
    auto mousePosition = windowToGuiCoords(mx, my);
    for (auto& layer : overlays) {
        layer->handleClick(button, mousePosition.x, mousePosition.y);
    }
}

void OverlayStack::handleMouseMove(sf::Event::MouseMoveEvent mouseMoveEvent)
{
    auto mousePosition = windowToGuiCoords(static_cast<float>(mouseMoveEvent.x),
                                           static_cast<float>(mouseMoveEvent.y));
    mouseMoveEvent.x = mousePosition.x;
    mouseMoveEvent.y = mousePosition.y;
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
    while (!m_pendingActions.empty()) {
        auto action = std::move(m_pendingActions.front());
        m_pendingActions.pop();

        switch (action.type) {
            case ActionType::PopLayer:
                if (overlays.size() > 0) {
                    overlays.pop_back();
                }
                break;

            case ActionType::PushLayer:
                overlays.push_back(std::move(action.overlay));
                break;

            default:
                break;
        }
    }
}

glm::vec2 OverlayStack::windowToGuiCoords(float winX, float winY) const
{
    return {winX / m_windowWidth * GUI_WIDTH, winY / m_windowHeight * GUI_HEIGHT};
}

} // namespace gui