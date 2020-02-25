#include "client_lua_callback.h"

#include <common/scripting/script_engine.h>

ClientLuaCallbacks::ClientLuaCallbacks(ScriptEngine& scriptEngine)
{
    scriptEngine.addCallbackApi("onClientStartup", m_onClientStartupCallbacks);

    scriptEngine.addFunction("onKeyReleased",
                             [this](sf::Keyboard::Key key, sol::function f) {
                                 m_onKeyReleaseCallbacks[key].push_back(f);
                             });
}

void ClientLuaCallbacks::onClientStartup()
{
    runLuaCallbacks(m_onClientStartupCallbacks);
}

void ClientLuaCallbacks::onKeyboardKeyReleased(sf::Keyboard::Key key)
{
    runLuaCallbacks(m_onKeyReleaseCallbacks[key]);
}