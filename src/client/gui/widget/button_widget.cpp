#include "button_widget.h"

#include "../component/rectangle_component.h"
#include "../component/text_component.h"
#include "widget_helper.h"

namespace gui {

    ButtonWidget::ButtonWidget(TextComponent* textComponent,
                               RectangleComponent* rectangleComponent)
        : mp_text(textComponent)
        , mp_rectangle(rectangleComponent)
    {
        componentList.push_back(mp_text);
        componentList.push_back(mp_rectangle);
    }

    void ButtonWidget::setPosition(const GuiDimension& position)
    {
        mp_rectangle->setPosition(position);
    }

    void ButtonWidget::setSize(const GuiDimension& size)
    {
        mp_rectangle->setSize(size);
    }

    void ButtonWidget::setImage(int image)
    {
        mp_rectangle->setTexture(image);
    }

    void ButtonWidget::setText(const std::string& text)
    {
        mp_text->setText(text);
    }

    void ButtonWidget::setTextSize(unsigned size)
    {
        mp_text->setFontSize(size);
    }

    void ButtonWidget::setColour(float r, float g, float b)
    {
        mp_rectangle->colour = {r, g, b};
    }

    void ButtonWidget::handleClick(sf::Mouse::Button button, float mx, float my)
    {
        if (mp_rectangle->isInBounds(mx, my) && button == sf::Mouse::Left) {
            if (m_onClick.valid()) {
                m_onClick();
            }
        }
    }

    void ButtonWidget::handleMouseMove(float mx, float my)
    {
        if (mp_rectangle->isInBounds(mx, my)) {
            if (m_onMoveOver.valid()) {
                m_onMoveOver();
            }
        }
        else {
            if (m_onMoveOver.valid()) {
                m_onMouseOff();
            }
        }
    }
    void ButtonWidget::setOnClick(sol::function function)
    {
        m_onClick = function;
    }

    void ButtonWidget::setOnMouseOver(sol::function function)
    {
        m_onMoveOver = function;
    }

    void ButtonWidget::setOnMouseOff(sol::function function)
    {
        m_onMouseOff = function;
    }

    void ButtonWidget::prepareRender()
    {
        mp_text->setPosition(centerText(mp_rectangle->getBounds(), *mp_text));
    }

} // namespace gui
