#pragma once

#include "widget.h"

namespace gui {
    /**
     * @brief Raw image overlay widget
     */
    class ImageWidget final : public Widget {
      public:
        ImageWidget(RectangleComponent* rectangleComponent);
        void setPosition(const GuiDimension& position) final override;
        void setSize(const GuiDimension& size) final override;

        void setImage(int image);

      private:
        RectangleComponent* mp_rectangle = nullptr;
    };
} // namespace gui
