#pragma once

#include "widget.h"

namespace gui {
    /**
     * @brief Raw text widget
     */
    class LabelWidget : public Widget {
      public:
        LabelWidget(TextComponent* textComponent);
        void setPosition(const GuiDimension& position) final override;
        void setSize(const GuiDimension& size) final override;

        void setText(const std::string& text);
        void setTextSize(unsigned size);

      protected:
        const TextComponent* getText() const;

      private:
        TextComponent* mp_text = nullptr;
    };

    class CenteredLabelWidget final : public LabelWidget {
      public:
        CenteredLabelWidget(TextComponent* textComponent);
        void prepareRender() final override;
    };

} // namespace gui
