#include "overlay.h"

#include "widget/button_widget.h"
#include "widget/checkbox_widget.h"
#include "widget/image_widget.h"
#include "widget/label_widget.h"
#include "widget/text_box_widget.h"
#include <iostream>

namespace gui {
    Overlay::Overlay(const OverlayDefinition& overlayDefinition)
        : definition(overlayDefinition)
    {
    }

    ImageWidget* Overlay::addImage()
    {
        auto rectangle =
            rectangleComponents.emplace_back(std::make_unique<RectangleComponent>())
                .get();
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

    CenteredLabelWidget* Overlay::addCenteredLabel()
    {
        auto text = textComponents.emplace_back(std::make_unique<TextComponent>()).get();
        auto label = std::make_unique<CenteredLabelWidget>(text);
        auto widget = m_widgets.emplace_back(std::move(label)).get();
        return dynamic_cast<CenteredLabelWidget*>(widget);
    }

    CheckBoxWidget* Overlay::addCheckBox()
    {
        auto rectangle =
            rectangleComponents.emplace_back(std::make_unique<RectangleComponent>())
                .get();
        auto label = textComponents.emplace_back(std::make_unique<TextComponent>()).get();
        auto checkBox = std::make_unique<CheckBoxWidget>(rectangle, label);
        auto widget = m_widgets.emplace_back(std::move(checkBox)).get();

        return dynamic_cast<CheckBoxWidget*>(widget);
    }

    ButtonWidget* Overlay::addButton()
    {
        auto rectangle =
            rectangleComponents.emplace_back(std::make_unique<RectangleComponent>())
                .get();
        auto text = textComponents.emplace_back(std::make_unique<TextComponent>()).get();

        auto button = std::make_unique<ButtonWidget>(text, rectangle);
        auto widget = m_widgets.emplace_back(std::move(button)).get();

        return dynamic_cast<ButtonWidget*>(widget);
    }

    TextBoxWidget* Overlay::addTextBox()
    {
        auto rectangle =
            rectangleComponents.emplace_back(std::make_unique<RectangleComponent>())
                .get();
        auto text = textComponents.emplace_back(std::make_unique<TextComponent>()).get();
        auto label = textComponents.emplace_back(std::make_unique<TextComponent>()).get();

        auto textbox = std::make_unique<TextBoxWidget>(text, rectangle, label);
        auto widget = m_widgets.emplace_back(std::move(textbox)).get();
        return dynamic_cast<TextBoxWidget*>(widget);
    }

    void Overlay::prepareWidgetsForRender()
    {
        for (auto& widget : m_widgets) {
            widget->prepareRender();
        }
    }

    void Overlay::handleClick(sf::Mouse::Button button, float mx, float my)
    {
        for (auto& widget : m_widgets) {
            widget->handleClick(button, mx, my);
        }
    }

    void Overlay::handleMouseMove(sf::Event::MouseMoveEvent mouseMoveEvent)
    {
        float mouseX = static_cast<float>(mouseMoveEvent.x);
        float mouseY = static_cast<float>(mouseMoveEvent.y);
        for (auto& widget : m_widgets) {
            widget->handleMouseMove(mouseX, mouseY);
        }
    }

    void Overlay::handleKeyRelease(sf::Keyboard::Key key)
    {
        for (auto& widget : m_widgets) {
            widget->handleKeyRelease(key);
        }
    }

    void Overlay::handleTextEntered(unsigned char keycode)
    {
        for (auto& widget : m_widgets) {
            widget->handleTextEntered(keycode);
        }
    }

    void Overlay::hide()
    {
        m_isHidden = true;
    }

    void Overlay::show()
    {
        m_isHidden = false;
    }

    bool Overlay::isHidden() const
    {
        return m_isHidden;
    }

} // namespace gui
