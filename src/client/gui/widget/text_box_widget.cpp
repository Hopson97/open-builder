#include "text_box_widget.h"

#include "../component/rectangle_component.h"
#include "../component/text_component.h"
#include "widget_helper.h"

namespace gui {

TextBoxWidget::TextBoxWidget(TextComponent* textComponent,
                             RectangleComponent* rectangleComponent)
    : mp_text(textComponent)
    , mp_rectangle(rectangleComponent)
{
    componentList.push_back(mp_text);
    componentList.push_back(mp_rectangle);
}

void TextBoxWidget::setPosition(const GuiDimension& position)
{
    mp_rectangle->setPosition(position);
}

void TextBoxWidget::setSize(const GuiDimension& size)
{
    mp_rectangle->setSize(size);
}

void TextBoxWidget::setImage(int image)
{
    mp_rectangle->setTexture(image);
}

void TextBoxWidget::setText(const std::string& text)
{
    m_textInput = text;
    mp_text->setText(text);
}

void TextBoxWidget::setTextSize(unsigned size)
{
    mp_text->setFontSize(size);
}

const std::string& TextBoxWidget::getText() const
{
    return m_textInput;
}

void TextBoxWidget::setColour(float r, float g, float b)
{
    mp_rectangle->colour = {r, g, b};
}

void TextBoxWidget::handleClick(sf::Mouse::Button button, float mx, float my)
{
    m_isActive = mp_rectangle->isInBounds(mx, my) && button == sf::Mouse::Left;
}

void TextBoxWidget::handleMouseMove(float mx, float my)
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

void TextBoxWidget::setOnMouseOver(sol::function function)
{
    m_onMoveOver = function;
}

void TextBoxWidget::setOnMouseOff(sol::function function)
{
    m_onMouseOff = function;
}

void TextBoxWidget::prepareRender()
{
    mp_text->setPosition(centerText(mp_rectangle->getBounds(), *mp_text));
}

} // namespace gui