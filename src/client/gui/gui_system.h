#pragma once

#include "../renderer/gui_renderer.h"
#include "overlay.h"
#include "overlay_factory.h"

class ScriptEngine;

namespace gui {

    /**
     * @brief Acts as an interface for the minimal gui api from the C++ side of the engine
     *
     */
    class GuiSystem {
      public:
        GuiSystem(unsigned windowWidth, unsigned windowHeight);

        void handleEvent(const sf::Event& event);
        void render(GuiRenderer& guiRenderer);

        // Lua API
        void addGuiDefintion(const gui::OverlayDefinition& def);
        void changeGui(const std::string& name, const std::string& data);

      private:
        glm::vec2 windowToGuiCoords(float winX, float winY) const;

        OverlayFactory m_overlayFactory;
        std::unique_ptr<Overlay> m_activeGui = nullptr;
        std::unique_ptr<Overlay> m_pendingGui = nullptr;

        unsigned m_windowWidth;
        unsigned m_windowHeight;
    };

} // namespace gui