#include "gui_system.h"

#include "../lua/client_lua_api.h"

namespace gui {
    GuiSystem::GuiSystem(unsigned windowWidth, unsigned windowHeight)
        : m_windowWidth(windowWidth)
        , m_windowHeight(windowHeight)
    {
    }

    void GuiSystem::handleEvent(const sf::Event& event)
    {
        if (m_activeGui) {
            switch (event.type) {
                case sf::Event::TextEntered:
                    m_activeGui->handleTextEntered(event.text.unicode);
                    break;

                case sf::Event::KeyReleased:
                    m_activeGui->handleKeyRelease(event.key.code);
                    break;

                case sf::Event::MouseMoved: {
                    auto mouseMoveEvent = event.mouseMove;
                    auto p = windowToGuiCoords(static_cast<float>(mouseMoveEvent.x),
                                               static_cast<float>(mouseMoveEvent.y));
                    mouseMoveEvent.x = static_cast<int>(p.x);
                    mouseMoveEvent.y = static_cast<int>(p.y);
                    m_activeGui->handleMouseMove(mouseMoveEvent);
                } break;

                case sf::Event::MouseButtonReleased: {
                    auto p = windowToGuiCoords(static_cast<float>(event.mouseButton.x),
                                               static_cast<float>(event.mouseButton.y));
                    m_activeGui->handleClick(event.mouseButton.button, p.x, p.y);
                } break;

                default:
                    break;
            }
        }
    }

    void GuiSystem::update()
    {
        if (m_pendingGui) {
            m_activeGui = std::move(m_pendingGui);
            m_pendingGui = nullptr;
        }
    }

    int GuiSystem::getActiveGuiRectCount() const
    {
        return m_activeGui->rectangleComponents.size();
    }

    int GuiSystem::getActiveGuiTextCount() const
    {
        return m_activeGui->textComponents.size();
    }

    void GuiSystem::changeGui(const std::string& name, const sol::table& data)
    {
        m_pendingGui = m_overlayFactory.createOverlay(name, data);
    }

    void GuiSystem::addGuiDefintion(const gui::OverlayDefinition& def)
    {
        m_overlayFactory.addOverlay(def);
    }

    void GuiSystem::render(GuiRenderer& guiRenderer)
    {
        if (m_activeGui) {
            if (!m_activeGui->isHidden()) {
                m_activeGui->prepareWidgetsForRender();
                guiRenderer.render(*m_activeGui);
            }
        }
    }

    glm::vec2 GuiSystem::windowToGuiCoords(float winX, float winY) const
    {
        return {winX / m_windowWidth * GUI_WIDTH, winY / m_windowHeight * GUI_HEIGHT};
    }
} // namespace gui
