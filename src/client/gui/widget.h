#pragma once

#include <SFML/Window/Event.hpp>

namespace gui {
struct GuiDimension;
class RectangleComponent;
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

  private:
    RectangleComponent* mp_rectangle;
};
} // namespace gui
