#include "gui_system.h"

#include "../client_config.h"
#include "../lua/client_lua_api.h"

namespace gui {
    GuiSystem::GuiSystem()
        : m_windowWidth(ClientConfig::get().windowWidth)
        , m_windowHeight(ClientConfig::get().windowHeight)
    {
    }

    void GuiSystem::handleEvent(const sf::Event& event)
    {
        if (!m_activeGuis.empty()) {
            auto& gui = getTop();
            switch (event.type) {
                case sf::Event::TextEntered:
                    gui.handleTextEntered(event.text.unicode);
                    break;

                case sf::Event::KeyReleased:
                    gui.handleKeyRelease(event.key.code);
                    break;

                case sf::Event::MouseMoved: {
                    auto mouseMoveEvent = event.mouseMove;
                    auto p = windowToGuiCoords(static_cast<float>(mouseMoveEvent.x),
                                               static_cast<float>(mouseMoveEvent.y));
                    mouseMoveEvent.x = static_cast<int>(p.x);
                    mouseMoveEvent.y = static_cast<int>(p.y);
                    gui.handleMouseMove(mouseMoveEvent);
                } break;

                case sf::Event::MouseButtonReleased: {
                    auto p = windowToGuiCoords(static_cast<float>(event.mouseButton.x),
                                               static_cast<float>(event.mouseButton.y));
                    gui.handleClick(event.mouseButton.button, p.x, p.y);
                } break;

                default:
                    break;
            }
        }
    }

    void GuiSystem::update()
    {
        if (m_nextAction != Action::None) {
            switch (m_nextAction) {
                case gui::GuiSystem::Action::Push:
                    m_activeGuis.push(std::move(m_pendingGui));
                    break;

                case gui::GuiSystem::Action::Pop:
                    m_activeGuis.pop();
                    break;

                case gui::GuiSystem::Action::Change:
                    clearGuis();
                    m_activeGuis.push(std::move(m_pendingGui));
                    break;

                default:
                    break;
            }
            m_pendingGui = nullptr;
            m_nextAction = Action::None;
        }
    }

    int GuiSystem::getActiveGuiRectCount() const
    {
        return getTop().rectangleComponents.size();
    }

    int GuiSystem::getActiveGuiTextCount() const
    {
        return getTop().textComponents.size();
    }

    void GuiSystem::changeGui(const std::string& name, const sol::table& data)
    {
        m_nextAction = Action::Change;
        m_pendingGui = m_overlayFactory.createOverlay(name, data);
    }

    void GuiSystem::pushGui(const std::string& name, const sol::table& data)
    {
        m_nextAction = Action::Push;
        m_pendingGui = m_overlayFactory.createOverlay(name, data);
    }

    void GuiSystem::popGui()
    {
        m_nextAction = Action::Pop;
    }

    void GuiSystem::addGuiDefintion(const gui::OverlayDefinition& def)
    {
        m_overlayFactory.addOverlay(def);
    }

    void GuiSystem::render(GuiRenderer& guiRenderer)
    {
        if (!m_activeGuis.empty()) {
            auto& gui = getTop();
            if (!gui.isHidden()) {
                gui.prepareWidgetsForRender();
                guiRenderer.render(gui);
            }
        }
    }

    void GuiSystem::clearGuis()
    {
        while (!m_activeGuis.empty()) {
            m_activeGuis.pop();
        }
    }

    Overlay& GuiSystem::getTop()
    {
        return *m_activeGuis.top();
    }

    const Overlay& GuiSystem::getTop() const
    {
        return *m_activeGuis.top();
    }

    glm::vec2 GuiSystem::windowToGuiCoords(float winX, float winY) const
    {
        return {winX / m_windowWidth * GUI_WIDTH, winY / m_windowHeight * GUI_HEIGHT};
    }
} // namespace gui
