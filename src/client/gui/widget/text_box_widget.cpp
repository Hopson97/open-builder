#include "text_box_widget.h"

#include "../component/rectangle_component.h"
#include "../component/text_component.h"
#include "widget_helper.h"

namespace {
bool isCharacterValid(unsigned char keyCode)
{
    return ((keyCode >= 48) && (keyCode <= 57)) ||  // Numbers
           ((keyCode >= 65) && (keyCode <= 90)) ||  // Uppercase
           ((keyCode >= 97) && (keyCode <= 122)) || // Lowercase
           keyCode == 32;                           // Space
}

bool isBackspace(unsigned char keycode)
{
    return keycode == 8;
}
} // namespace

namespace gui {

TextBoxWidget::TextBoxWidget(TextComponent* textComponent,
                             RectangleComponent* rectangleComponent, TextComponent* label)
    : mp_text(textComponent)
    , mp_rectangle(rectangleComponent)
    , mp_label(label)
{
    componentList.push_back(mp_text);
    componentList.push_back(mp_rectangle);
    componentList.push_back(mp_label);

    mp_label->setFontSize(30);
    mp_label->setText("Test");
}

void TextBoxWidget::setPosition(const GuiDimension& position)
{
    GuiDimension textPosition = position;
    textPosition.offset.y += mp_rectangle->getBounds().height;

    mp_label->setPosition(textPosition);
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

void TextBoxWidget::setPlaceholder(const std::string& text)
{
    m_placeholder = text;
    mp_text->setText(text);
    m_isUsingPlaceholder = true;
}

void TextBoxWidget::setMaxLength(int length)
{
    m_maxLength = length;
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

void TextBoxWidget::handleTextEntered(unsigned char code)
{
    if (m_isActive) {
        if (isCharacterValid(code) && m_textInput.length() < m_maxLength) {
            m_textInput.push_back(code);
            mp_text->setText(m_textInput);
            m_isUsingPlaceholder = false;
        }
        else if (isBackspace(code) && !m_textInput.empty()) {
            m_textInput.pop_back();
            m_isUsingPlaceholder = m_textInput.empty();
            mp_text->setText(m_isUsingPlaceholder ? m_placeholder : m_textInput);
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
    mp_text->colour =
        m_isUsingPlaceholder ? glm::vec3{0.65, 0.65, 0.65} : glm::vec3{1.0, 1.0, 1.0f};
}

} // namespace gui