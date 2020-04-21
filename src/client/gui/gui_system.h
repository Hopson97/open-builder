#pragma once

#include "../renderer/gui_renderer.h"
#include "overlay.h"
#include "overlay_factory.h"

#include <stack>

struct ScriptEngine;

namespace gui {

    /**
     * @brief Acts as an interface for the minimal gui api from the C++ side of the engine
     *
     */
    class GuiSystem {
        enum class Action {
            None,
            Push,
            Pop,
            Change,
        };

      public:
        GuiSystem();

        void handleEvent(const sf::Event& event);
        void update();
        void render(GuiRenderer& guiRenderer);

        // Debugging/ Testing purposes
        int getActiveGuiRectCount() const;
        int getActiveGuiTextCount() const;

        // Lua API
        void addGuiDefintion(const gui::OverlayDefinition& def);
        void changeGui(const std::string& name, const sol::table& data);
        void pushGui(const std::string& name, const sol::table& data);
        void popGui();

      private:
        void clearGuis();
        Overlay& getTop();
        const Overlay& getTop() const;
        glm::vec2 windowToGuiCoords(float winX, float winY) const;

        OverlayFactory m_overlayFactory;
        std::stack<std::unique_ptr<Overlay>> m_activeGuis;
        std::unique_ptr<Overlay> m_pendingGui = nullptr;

        Action m_nextAction = Action::None;

        unsigned m_windowWidth;
        unsigned m_windowHeight;
    };

} // namespace gui
