#include "overlay_stack.h"

namespace gui {

    OverlayStack::OverlayStack(unsigned winWidth, unsigned winHeight)
        : m_windowWidth(static_cast<float>(winWidth))
        , m_windowHeight(static_cast<float>(winHeight))
    {
    }

    void OverlayStack::resetToLayer(std::unique_ptr<Overlay> layer)
    {
        Action action;
        action.type = ActionType::ResetLayers;
        action.overlay = std::move(layer);
        m_pendingActions.push(std::move(action));
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

    void OverlayStack::handleEvent(const sf::Event& event)
    {
        switch (event.type) {
            case sf::Event::TextEntered:
                forEachOverlay([&](auto& overlay) {
                    overlay->handleTextEntered(event.text.unicode);
                });
                break;

            case sf::Event::KeyReleased:
                forEachOverlay(
                    [&](auto& overlay) { overlay->handleKeyRelease(event.key.code); });
                break;

            case sf::Event::MouseMoved:
                handleMouseMove(event.mouseMove);
                break;

            case sf::Event::MouseButtonReleased:
                handleClick(event.mouseButton.button,
                            static_cast<float>(event.mouseButton.x),
                            static_cast<float>(event.mouseButton.y));
                break;

            default:
                break;
        }
    }

    void OverlayStack::handleClick(sf::Mouse::Button button, float mx, float my)
    {
        auto mousePosition = windowToGuiCoords(mx, my);
        for (auto& layer : overlays) {
            if (!layer->isHidden()) {
                layer->handleClick(button, mousePosition.x, mousePosition.y);
            }
        }
    }

    void OverlayStack::handleMouseMove(sf::Event::MouseMoveEvent mouseMoveEvent)
    {
        auto mousePosition = windowToGuiCoords(static_cast<float>(mouseMoveEvent.x),
                                               static_cast<float>(mouseMoveEvent.y));
        mouseMoveEvent.x = static_cast<int>(mousePosition.x);
        mouseMoveEvent.y = static_cast<int>(mousePosition.y);
        for (auto& layer : overlays) {
            if (!layer->isHidden()) {
                layer->handleMouseMove(mouseMoveEvent);
            }
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
                    if (overlays.size() > 0) {
                        overlays.back()->show();
                    }
                    break;

                case ActionType::PushLayer:
                    if (overlays.size() > 0) {
                        overlays.back()->hide();
                    }
                    overlays.push_back(std::move(action.overlay));
                    break;

                case ActionType::ResetLayers:
                    while (!overlays.empty()) {
                        overlays.pop_back();
                    }
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