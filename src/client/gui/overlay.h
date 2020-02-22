#pragma once

#include "component.h"
#include "widget.h"
#include <memory>
#include <sol/sol.hpp>
#include <unordered_map>
#include <vector>

namespace gui {

struct OverlayDefinition final {
    std::string id;
    std::string title;

    sol::function create;
};

struct Overlay final {
    Overlay(const OverlayDefinition& overlayDefinition);

    void handleClick(sf::Mouse::Button);
    void handleMouseMove(sf::Event::MouseMoveEvent);
    void handleKeyRelease(sf::Keyboard::Key);

    void addRectangle();

    const OverlayDefinition& definition;

    std::vector<std::unique_ptr<Widget>> widgets;

    // Stored as pointers to allow them to be stored by the Lua code
    std::vector<std::unique_ptr<RectangleComponent>> rectangleComponent;
    std::vector<std::unique_ptr<RectangleComponent>> textComponents;
};

struct OverlayStack final {
    void pushLayer(std::unique_ptr<Overlay>);
    void popLayer();
    void removeLayerByName(const std::string& overlayId);

    void handleClick(sf::Mouse::Button);
    void handleMouseMove(sf::Event::MouseMoveEvent);
    void handleKeyRelease(sf::Keyboard::Key);

    std::vector<std::unique_ptr<Overlay>> overlays;
};

class OverlayFactory {
  public:
    void addOverlay(const OverlayDefinition& overlay);
    std::unique_ptr<Overlay> createOverlay(const std::string& name);

  private:
    std::vector<OverlayDefinition> m_overlays;
    std::unordered_map<std::string, int> m_overlayMap;
};

} // namespace gui
