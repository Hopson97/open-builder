#include "widget.h"

#include "../component/text_component.h"

namespace gui {

LabelWidget::LabelWidget(TextComponent* textComponent)
    : mp_text(textComponent)
{
}
void LabelWidget::setPosition(const GuiDimension& position)
{
    mp_text->setPosition(position);
}

void LabelWidget::setSize(const GuiDimension& size)
{
    mp_text->setFontSize(size.offset.x);
}

void LabelWidget::setText(const std::string& text)
{
    mp_text->setText(text);
}

void LabelWidget::setTextSize(float size)
{
    mp_text->setFontSize(size);
}

} // namespace gui
