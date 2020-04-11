#pragma once

struct ScriptEngine;

#include <SFML/Window/Window.hpp>

namespace gui {
    class GuiSystem;
} // namespace gui

class GuiRenderer;
struct InputState;
class ClientStateController;

/**
 * @brief Init the overlay APIs
 *
 * @param scriptEngine The Lua script engine/ lua state to append the API to
 * @param guiSystem The client's GUI
 * @param guiRenderer The client's GUI Renderer; optional
 */
void luaInitGuiApi(ScriptEngine& scriptEngine, gui::GuiSystem& guiSystem,
                   GuiRenderer* guiRenderer = nullptr);

/**
 * @brief Init API for the various Open Builder widgets, (Buttons, etc)
 *
 * @param scriptEngine The Lua script engine/ lua state to append the API to
 */
void luaInitGuiWidgetApi(ScriptEngine& scriptEngine);

/**
 * @brief Init the input api, eg key bindings and mouse locking
 *
 * @param scriptEngine The Lua script engine/ lua state to append the API to
 * @param window The window used for certain control (eg locking or showing mouse)
 * @param inputState The input state for furthur input control
 */
void luaInitInputApi(ScriptEngine& scriptEngine, sf::Window& window,
                     InputState& inputState);

/**
 * @brief Init control api, allowing guis and such to exit the game, start the game, load
 * a world etc
 *
 * @param scriptEngine The Lua script engine/ lua state to append the API to
 * @param clientState The state controller
 */
void luaInitClientControlApi(ScriptEngine& scriptEngine,
                             ClientStateController& clientState);
