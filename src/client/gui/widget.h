#pragma once

#include <SFML/Window/Event.hpp>

namespace gui {
struct GuiDimension;
class RectangleComponent;
class TextComponent;

struct Widget {
    virtual void handleClick(sf::Mouse::Button){};
    virtual void handleMouseMove(sf::Event::MouseMoveEvent){};
    virtual void handleKeyRelease(sf::Keyboard::Key){};

    virtual void setPosition(const GuiDimension& position) = 0;
    virtual void setSize(const GuiDimension& size) = 0;
};

class ImageWidget final : public Widget {
  public:
    ImageWidget(RectangleComponent* rectangleComponent);
    void setPosition(const GuiDimension& position) final override;
    void setSize(const GuiDimension& size) final override;

    void setImage(int image);

  private:
    RectangleComponent* mp_rectangle;
};

class LabelWidget final : public Widget {
  public:
    LabelWidget(TextComponent* textComponent);
    void setPosition(const GuiDimension& position) final override;
    void setSize(const GuiDimension& size) final override;

    void setText(const std::string& text);
    void setTextSize(float size);

  private:
    TextComponent* mp_text;
};
} // namespace gui
