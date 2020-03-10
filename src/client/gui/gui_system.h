#pragma once

#include "../renderer/gui_renderer.h"
#include "overlay_factory.h"
#include "overlay_stack.h"

class ScriptEngine;

namespace gui {

    /**
     * @brief Acts as an interface for the minimal gui api from the C++ side of the engine
     *
     */
    class GuiSystem {
      public:
        GuiSystem(unsigned windowWidth, unsigned windowHeight,
                  ScriptEngine& scriptEngine);

        void handleEvent(const sf::Event& event);
        void update();
        void render();

      private:
        OverlayFactory m_overlayFactory;
        OverlayStack m_overlayStack;
        GuiRenderer m_guiRenderer;
    };

} // namespace gui