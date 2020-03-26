#include "checkbox_widget.h"

#include "../component/rectangle_component.h"
#include "../component/text_component.h"
#include "widget_helper.h"

namespace gui {

    CheckBoxWidget::CheckBoxWidget(RectangleComponent* rectComponent, TextComponent* label)
        : mp_rectangle(rectComponent)
        , mp_label(label)
        , isChecked(false)
    {
        componentList.push_back(mp_rectangle);
        componentList.push_back(mp_label);

        mp_label->setFontSize(40);
    }

    void CheckBoxWidget::setPosition(const GuiDimension& position)
    {
        //GuiDimension textPosition = position;
        //textPosition.offset.y += mp_rectangle->getBounds().height;

        //mp_label->setPosition(textPosition);
        mp_rectangle->setPosition(position);
    }

    void CheckBoxWidget::setSize(const GuiDimension& size)
    {
        mp_rectangle->setSize(size);
    }

    void CheckBoxWidget::setColour(float r, float g, float b)
    {
        mp_rectangle->colour = {r, g, b};
    }

    void CheckBoxWidget::setImage(int Image)
    {
        mp_rectangle->setTexture(Image);
    }

    void CheckBoxWidget::setText(const std::string& text)
    {
        mp_label->setText(text);
    }

    void CheckBoxWidget::setTextSize(unsigned size)
    {
        mp_label->setFontSize(size);
    }

    void CheckBoxWidget::handleClick(sf::Mouse::Button button, float mx, float my)
    {
        if (mp_rectangle->isInBounds(mx, my) && button == sf::Mouse::Left) {
            if (m_onClick.valid()) {
                m_onClick();
            }
        }
    }

    void CheckBoxWidget::handleMouseMove(float mx, float my)
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

    void CheckBoxWidget::setOnClick(sol::function function)
    {
        m_onClick = function;
    }

    void CheckBoxWidget::setOnMouseOver(sol::function function)
    {
        m_onMoveOver = function;
    }

    void CheckBoxWidget::setOnMouseOff(sol::function function)
    {
        m_onMouseOff = function;
    }

    void CheckBoxWidget::prepareRender()
    {
        GuiDimension pos = centerText(mp_rectangle->getBounds(), *mp_label);
        pos.offset.x -= mp_rectangle->getBounds().width / 6;

        mp_label->setPosition(pos);
    }


} // namespace gui