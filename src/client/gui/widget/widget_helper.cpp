#include "widget_helper.h"

#include "../component/component.h"
#include "../component/text_component.h"

namespace gui {

    GuiDimension centerText(const sf::FloatRect& bounds, const TextComponent& text)
    {
        const auto& size = text.getSize();

        float left = bounds.left + bounds.width / 2 - size.x / 2;
        float top = GUI_HEIGHT - bounds.top - bounds.height / 2 - size.y / 4;

        return {0, left, 0, top};
    }

} // namespace gui
