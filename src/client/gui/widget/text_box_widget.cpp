#include "text_box_widget.h"

#include "../component/rectangle_component.h"
#include "../component/text_component.h"
#include "widget_helper.h"

namespace gui {

TextboxWidget::TextboxWidget(TextComponent* textComponent,
                             RectangleComponent* rectangleComponent)
    : mp_text(textComponent)
    , mp_rectangle(rectangleComponent)
{
    componentList.push_back(mp_text);
    componentList.push_back(mp_rectangle);
}

void TextboxWidget::setPosition(const GuiDimension& position)
{
    mp_rectangle->setPosition(position);
}

void TextboxWidget::setSize(const GuiDimension& size)
{
    mp_rectangle->setSize(size);
}

void TextboxWidget::setImage(int image)
{
    mp_rectangle->setTexture(image);
}

void TextboxWidget::setText(const std::string& text)
{
    m_textInput = text;
    mp_text->setText(text);
}

void TextboxWidget::setTextSize(unsigned size)
{
    mp_text->setFontSize(size);
}

const std::string& TextboxWidget::getText() const
{
    return m_textInput;
}

void TextboxWidget::setColour(float r, float g, float b)
{
    mp_rectangle->colour = {r, g, b};
}

void TextboxWidget::handleClick(sf::Mouse::Button button, float mx, float my)
{
    m_isActive = mp_rectangle->isInBounds(mx, my) && button == sf::Mouse::Left;
}

void TextboxWidget::handleMouseMove(float mx, float my)
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

void TextboxWidget::setOnMouseOver(sol::function function)
{
    m_onMoveOver = function;
}

void TextboxWidget::setOnMouseOff(sol::function function)
{
    m_onMouseOff = function;
}

void TextboxWidget::prepareRender()
{
    mp_text->setPosition(centerText(mp_rectangle->getBounds(), *mp_text));
}

} // namespace gui