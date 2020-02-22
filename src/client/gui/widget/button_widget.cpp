#include "widget.h"

#include <iostream>

#include "../component/rectangle_component.h"
#include "../component/text_component.h"

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
    mp_text->setPosition(position);
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

void ButtonWidget::setTextSize(float size)
{
    mp_text->setFontSize(size);
}

void ButtonWidget::handleClick(sf::Mouse::Button button, float mx, float my)
{
    if (mp_rectangle->isInBounds(mx, my) && button == sf::Mouse::Left) {
        if (m_onClick.valid()) {
            m_onClick();
        }
    }
}

void ButtonWidget::handleMouseMove(sf::Event::MouseMoveEvent event)
{
    if (mp_rectangle->isInBounds(event.x, event.y)) {
        if (m_onMoveOver.valid()) {
            m_onMoveOver();
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

} // namespace gui
