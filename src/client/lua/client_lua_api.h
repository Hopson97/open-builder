#pragma once

struct ScriptEngine;

#include <SFML/Window/Window.hpp>

namespace gui {
class OverlayFactory;
class OverlayStack;
} // namespace gui

class GuiRenderer;
struct InputState;

/**
 * @brief Init the overlay APIs
 *
 * @param scriptEngine The Lua script engine/ lua state to append the API to
 * @param overlayFactory The client's GUI Overlay Factory
 * @param overlayStack The client's GUI overlay stack
 * @param guiRenderer The client's GUI Renderer; optional
 */
void luaInitGuiApi(ScriptEngine& scriptEngine, gui::OverlayFactory& overlayFactory,
                   gui::OverlayStack& overlayStack, GuiRenderer* guiRenderer = nullptr);

/**
 * @brief Init API for the various Open Builder widgets, (Buttons, etc)
 *
 * @param scriptEngine The Lua script engine/ lua state to append the API to
 */
void luaInitGuiWidgetApi(ScriptEngine& scriptEngine);

/**
 * @brief
 *
 * @param scriptEngine The Lua script engine/ lua state to append the API to
 * @param window The window used for certain control (eg locking or showing mouse)
 * @param inputState The input state for furthur input control
 */
void luaInitInputApi(ScriptEngine& scriptEngine, sf::Window& window,
                     InputState& inputState);
