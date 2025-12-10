#include "widget_helper.h"

#include "../component/component.h"
#include "../component/text_component.h"

namespace gui {

    GuiDimension centerText(const sf::FloatRect& bounds, const TextComponent& text)
    {
        const auto& size = text.getSize();

        float left = bounds.position.x + bounds.size.x / 2 - size.x / 2;
        float top = GUI_HEIGHT - bounds.position.y - bounds.size.y / 2 - size.y / 4;

        return {0, left, 0, top};
    }

} // namespace gui
