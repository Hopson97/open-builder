#pragma once

#include "widget.h"

namespace gui {
/**
 * @brief Raw text widget
 */
class LabelWidget final : public Widget {
  public:
    LabelWidget(TextComponent* textComponent);
    void setPosition(const GuiDimension& position) final override;
    void setSize(const GuiDimension& size) final override;

    void setText(const std::string& text);
    void setTextSize(float size);

  private:
    TextComponent* mp_text;
    RectangleComponent* mp_rectangle;
};
} // namespace gui
