#pragma once

#include <SFML/Graphics/Rect.hpp>

namespace gui {
    struct GuiDimension;
    class TextComponent;

    GuiDimension centerText(const sf::FloatRect& bounds, const TextComponent& text);

} // namespace gui
