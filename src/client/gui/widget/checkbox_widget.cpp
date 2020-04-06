#include "checkbox_widget.h"

#include "../component/rectangle_component.h"
#include "../component/text_component.h"
#include "widget_helper.h"

namespace gui {

    CheckBoxWidget::CheckBoxWidget(RectangleComponent* rectComponent,
                                   TextComponent* label)
        : mp_rectangle(rectComponent)
        , mp_label(label)
    {
        componentList.push_back(mp_rectangle);
        componentList.push_back(mp_label);

        mp_label->setFontSize(40);
    }

    void CheckBoxWidget::setPosition(const GuiDimension& position)
    {
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

    void CheckBoxWidget::setUncheckedImage(int Image)
    {
        uncheckedTexture = Image;
    }

    void CheckBoxWidget::setCheckedImage(int Image)
    {
        checkedTexture = Image;
    }

    void CheckBoxWidget::setText(const std::string& text)
    {
        mp_label->setText(text);
    }

    void CheckBoxWidget::setTextSize(unsigned size)
    {
        mp_label->setFontSize(size);
    }

    void CheckBoxWidget::setChecked(bool checked)
    {
        m_checked = checked;
    }

    void CheckBoxWidget::handleClick(sf::Mouse::Button button, float mx, float my)
    {
        if (mp_rectangle->isInBounds(mx, my) && button == sf::Mouse::Left) {
            onClick();
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

    void CheckBoxWidget::onClick()
    {
        m_checked = !m_checked;
        if (m_checked)
            setImage(checkedTexture);
        else
            setImage(uncheckedTexture);
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

    bool CheckBoxWidget::getChecked()
    {
        return m_checked;
    }

} // namespace gui
