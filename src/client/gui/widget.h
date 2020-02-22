#pragma once

#include <SFML/Window/Event.hpp>
#include <sol/sol.hpp>

namespace gui {
struct GuiDimension;
class RectangleComponent;
class TextComponent;

/**
 * @brief Base class for different widgets that can makeup a GUI overlay
 */
struct Widget {
    virtual ~Widget() = default;
    virtual void handleClick(sf::Mouse::Button, float, float){};
    virtual void handleMouseMove(sf::Event::MouseMoveEvent){};
    virtual void handleKeyRelease(sf::Keyboard::Key){};

    virtual void setPosition(const GuiDimension& position) = 0;
    virtual void setSize(const GuiDimension& size) = 0;
};

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
    RectangleComponent* mp_rectangle;
};

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

class ButtonWidget final : public Widget {
  public:
    ButtonWidget(TextComponent* textComponent, RectangleComponent* mp_rectangle);
    void setPosition(const GuiDimension& position) final override;
    void setSize(const GuiDimension& size) final override;

    void setImage(int image);

    void setText(const std::string& text);
    void setTextSize(float size);

    void handleClick(sf::Mouse::Button button, float mx, float my) final override;
    void handleMouseMove(sf::Event::MouseMoveEvent) final override;

    void setOnClick(sol::function function);
    void setOnMouseOver(sol::function function);

  private:
    TextComponent* mp_text;
    RectangleComponent* mp_rectangle;

    sol::function m_onClick;
    sol::function m_onMoveOver;
};
} // namespace gui
