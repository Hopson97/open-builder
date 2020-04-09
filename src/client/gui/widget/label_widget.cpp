#include "label_widget.h"

#include "../component/text_component.h"

namespace gui {

    LabelWidget::LabelWidget(TextComponent* textComponent)
        : mp_text(textComponent)
    {
        componentList.push_back(mp_text);
    }

    void LabelWidget::setPosition(const GuiDimension& position)
    {
        mp_text->setPosition(position);
    }

    void LabelWidget::setSize(const GuiDimension& size)
    {
        mp_text->setFontSize(static_cast<unsigned>(size.offset.x));
    }

    void LabelWidget::setText(const std::string& text)
    {
        mp_text->setText(text);
    }

    void LabelWidget::setTextSize(unsigned size)
    {
        mp_text->setFontSize(size);
    }

    const TextComponent* LabelWidget::getText() const
    {
        return mp_text;
    }

    CenteredLabelWidget::CenteredLabelWidget(TextComponent* textComponent)
        : LabelWidget(textComponent)
    {
    }

    void CenteredLabelWidget::prepareRender()
    {
        float y = getText()->getPosition().offset.y;
        float textWidth = getText()->getSize().x;
        float x = GUI_WIDTH / 2 - textWidth / 2;

        LabelWidget::setPosition({0, x, 0, y});
    }

} // namespace gui
