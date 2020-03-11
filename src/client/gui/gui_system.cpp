#include "gui_system.h"

#include "../lua/client_lua_api.h"

namespace gui {
    GuiSystem::GuiSystem(unsigned windowWidth, unsigned windowHeight,
                         ScriptEngine& scriptEngine)
        : m_overlayStack(windowWidth, windowHeight)
    {
        luaInitGuiApi(scriptEngine, m_overlayFactory, m_overlayStack, &m_guiRenderer);
    }

    void GuiSystem::handleEvent(const sf::Event& event)
    {
        m_overlayStack.handleEvent(event);
    }

    void GuiSystem::update()
    {
        m_overlayStack.update();
    }

    void GuiSystem::render()
    {
        for (auto& overlay : m_overlayStack.overlays) {
            if (!overlay->isHidden()) {
                overlay->prepareWidgetsForRender();
                m_guiRenderer.render(*overlay);
            }
        }
    }
} // namespace gui
