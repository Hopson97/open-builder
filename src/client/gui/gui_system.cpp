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

/**
 * @brief The 'F3' prompt showing info like # of chunks
 * drawn, frame time, etc
 *
struct DebugGui {
    gui::OverlayDefinition def;
    gui::Overlay overlay;

    gui::LabelWidget& label;

    DebugGui()
        : overlay(def)
        , label(*overlay.addLabel())
    {
        label.setPosition({0, 5, 0, GUI_HEIGHT - 25});
        label.setTextSize(32);
        label.setText("TESTING");
    }
};*/