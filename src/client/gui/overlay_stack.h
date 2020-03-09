#pragma once

#include "overlay.h"
#include <SFML/Window/Event.hpp>
#include <memory>
#include <queue>
#include <vector>

namespace gui {
    /**
     * @brief Stores the current overlays of the game
     */
    class OverlayStack final {
        enum class ActionType {
            None,
            PushLayer,
            PopLayer,
            ResetLayers,
        };

        struct Action final {
            ActionType type = ActionType::None;
            std::string id;
            std::unique_ptr<Overlay> overlay;
        };

      public:
        OverlayStack(unsigned winWidth, unsigned winHeight);

        void resetToLayer(std::unique_ptr<Overlay>);
        void pushLayer(std::unique_ptr<Overlay>);
        void popLayer();
        void removeLayerByName(const std::string& overlayId);

        void handleClick(sf::Mouse::Button button, float mx, float my);
        void handleMouseMove(sf::Event::MouseMoveEvent);
        void handleKeyRelease(sf::Keyboard::Key);
        void handleTextEntered(unsigned char keycode);

        void update();

        std::vector<std::unique_ptr<Overlay>> overlays;

      private:
        glm::vec2 windowToGuiCoords(float winX, float winY) const;

        const float m_windowWidth;
        const float m_windowHeight;
        std::queue<Action> m_pendingActions;
    };
} // namespace gui
