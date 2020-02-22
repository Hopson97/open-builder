#pragma once

#include "overlay.h"
#include <SFML/Window/Event.hpp>
#include <memory>
#include <sol/sol.hpp>
#include <unordered_map>
#include <vector>

namespace gui {

struct OverlayDefinition {
    std::string id;
    std::string title;

    sol::function create;
};

struct Widget {
    virtual void handleClick(sf::Mouse::Button){};
    virtual void handleMouseMove(sf::Event::MouseMoveEvent){};
    virtual void handleKeyRelease(sf::Keyboard::Key){};
};

struct Overlay {
    Overlay(const OverlayDefinition& overlayDefinition);

    void handleClick(sf::Mouse::Button);
    void handleMouseMove(sf::Event::MouseMoveEvent);
    void handleKeyRelease(sf::Keyboard::Key);

    void addRectangle();

    const OverlayDefinition& definition;

    std::vector<std::unique_ptr<Widget>> widgets;
};

struct OverlayStack {
    void pushLayer(std::unique_ptr<Overlay>);
    void popLayer();
    void removeLayerByName(const std::string& overlayId);

    void handleClick(sf::Mouse::Button);
    void handleMouseMove(sf::Event::MouseMoveEvent);
    void handleKeyRelease(sf::Keyboard::Key);

    std::vector<std::unique_ptr<Overlay>> m_overlayStack;
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
