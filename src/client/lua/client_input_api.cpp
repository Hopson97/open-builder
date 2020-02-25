#include "client_lua_api.h"

#include "../input/input_state.h"
#include <common/scripting/script_engine.h>

namespace {
void initMouseControlApi(sol::table& inputTable, sf::Window& window,
                         InputState& inputState)
{
    inputTable["hideMouse"] = [&window]() { window.setMouseCursorVisible(false); };
    inputTable["showMouse"] = [&window]() { window.setMouseCursorVisible(true); };

    inputTable["toggleMouseLock"] = [&inputState]() {
        inputState.isMouseLocked = !inputState.isMouseLocked;
    };
    inputTable["isMouseLocked"] = [&inputState]() { return inputState.isMouseLocked; };
    inputTable["lockMouse"] = [&inputState]() { inputState.isMouseLocked = true; };
    inputTable["unlockMouse"] = [&inputState]() { inputState.isMouseLocked = false; };
}
} // namespace

void luaInitInputApi(ScriptEngine& scriptEngine, sf::Window& window,
                     InputState& inputState)
{
    auto inputTable = scriptEngine.addTable("input");
    initMouseControlApi(inputTable, window, inputState);
}