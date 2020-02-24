#pragma once

#include "component/rectangle_component.h"
#include "component/text_component.h"
#include "widget/widget.h"
#include <memory>
#include <queue>
#include <sol/sol.hpp>
#include <unordered_map>
#include <vector>

namespace gui {

/**
 * @brief Defines how a GUI should be created.
 * This is created in the Client Lua code
 */
struct OverlayDefinition final {
    std::string id;
    std::string title;

    sol::function create;
};

/**
 * @brief Holds the components that make up some GUI overlay, eg HUD, menus etc
 *
 */
class Overlay final {
  public:
    Overlay(const OverlayDefinition& overlayDefinition);

    void handleClick(sf::Mouse::Button button, float mx, float my);
    void handleMouseMove(sf::Event::MouseMoveEvent);
    void handleKeyRelease(sf::Keyboard::Key);

    // These widget adding function return non-owning pointers
    // They are mostly called via Lua code, which is why it is pointers
    ImageWidget* addImage();
    LabelWidget* addLabel();
    ButtonWidget* addButton();

    // The overlay defintion that created this overlay
    const OverlayDefinition& definition;

    // Stored as pointers to allow them to be stored by the Lua code
    std::vector<std::unique_ptr<RectangleComponent>> rectangleComponents;
    std::vector<std::unique_ptr<TextComponent>> textComponents;

    void prepareWidgetsForRender();

    int widgetCount() const;

  private:
    std::vector<std::unique_ptr<Widget>> m_widgets;
};

/**
 * @brief Stores the current overlays of the game
 */
class OverlayStack final {
    enum class ActionType { PushLayer, PopLayer };

    struct Action {
        ActionType type;
        std::string id;
        std::unique_ptr<Overlay> overlay;
    };

  public:
    OverlayStack(float winWidth, float winHeight);

    void pushLayer(std::unique_ptr<Overlay>);
    void popLayer();
    void removeLayerByName(const std::string& overlayId);

    void handleClick(sf::Mouse::Button button, float mx, float my);
    void handleMouseMove(sf::Event::MouseMoveEvent);
    void handleKeyRelease(sf::Keyboard::Key);

    void update();

    std::vector<std::unique_ptr<Overlay>> overlays;

  private:
    glm::vec2 windowToGuiCoords(float winX, float winY) const;

    const float m_windowWidth;
    const float m_windowHeight;
    std::queue<Action> m_pendingActions;
};

/**
 * @brief Stores overlay defintions, and creates overlays using them
 *
 */
class OverlayFactory {
  public:
    /**
     * @brief Add a new overlay definition to the game
     * @param overlay The new overlay
     */
    void addOverlay(const OverlayDefinition& overlay);

    /**
     * @brief Create a Overlay object from a given overlay definition
     *
     * @param name The "overlay definition id", as defined in the Lua code
     * @return std::unique_ptr<Overlay> A newly created overlay, as a pointer such that
     * the Lua is able to store it without risk
     */
    std::unique_ptr<Overlay> createOverlay(const std::string& name);

  private:
    std::vector<OverlayDefinition> m_overlays;
    std::unordered_map<std::string, int> m_overlayMap;
};

} // namespace gui
