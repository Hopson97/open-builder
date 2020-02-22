#include "overlay.h"

namespace gui {
Overlay::Overlay(const OverlayDefinition& overlayDefinition)
    : definition(overlayDefinition)
{
}

ImageWidget* Overlay::addImage()
{
    auto rectangle =
        rectangleComponents.emplace_back(std::make_unique<RectangleComponent>()).get();
    auto image = std::make_unique<ImageWidget>(rectangle);
    auto widget = m_widgets.emplace_back(std::move(image)).get();
    return dynamic_cast<ImageWidget*>(widget);
}

LabelWidget* Overlay::addLabel()
{
    auto text = textComponents.emplace_back(std::make_unique<TextComponent>()).get();
    auto label = std::make_unique<LabelWidget>(text);
    auto widget = m_widgets.emplace_back(std::move(label)).get();
    return dynamic_cast<LabelWidget*>(widget);
}

void Overlay::handleClick(sf::Mouse::Button button)
{
    for (auto& widget : m_widgets) {
        widget->handleClick(button);
    }
}

void Overlay::handleMouseMove(sf::Event::MouseMoveEvent mouseMoveEvent)
{
    for (auto& widget : m_widgets) {
        widget->handleMouseMove(mouseMoveEvent);
    }
}

void Overlay::handleKeyRelease(sf::Keyboard::Key key)
{
    for (auto& widget : m_widgets) {
        widget->handleKeyRelease(key);
    }
}

int Overlay::widgetCount() const
{
    return m_widgets.size();
}
} // namespace gui
