#include "client_lua_callback.h"

#include <common/lua/script_engine.h>

void ClientLuaCallbacks::initCallbacks(ScriptEngine& scriptEngine)
{
    scriptEngine.addCallbackApi("onClientStartup", m_onClientStartupCallbacks);
    scriptEngine.addCallbackApi("onEnterGame", m_onEnterGameCallbacks);
    scriptEngine.addCallbackApi("onExitGame", m_onExitGameCallbacks);
    scriptEngine.addCallbackApi("onError", m_onErrorCallbacks);
    scriptEngine.addCallbackApi("onMouseWheelScroll", m_onMouseWheelScroll);

    scriptEngine.addFunction("onKeyReleased",
                             [this](sf::Keyboard::Key key, sol::function f) {
                                 m_onKeyReleaseCallbacks[(int)key].push_back(f);
                             });
}

void ClientLuaCallbacks::onClientStartup()
{
    runLuaCallbacks(m_onClientStartupCallbacks);
}

void ClientLuaCallbacks::onEnterGame()
{
    runLuaCallbacks(m_onEnterGameCallbacks);
}

void ClientLuaCallbacks::onExitGame()
{
    runLuaCallbacks(m_onExitGameCallbacks);
}

void ClientLuaCallbacks::onError(const std::string& error)
{
    runLuaCallbacks(m_onErrorCallbacks, error);
}

void ClientLuaCallbacks::onKeyboardKeyReleased(sf::Keyboard::Key key)
{
    if ((int)key < m_onKeyReleaseCallbacks.size())
    {
        runLuaCallbacks(m_onKeyReleaseCallbacks[(int)key]);
    }
}

void ClientLuaCallbacks::onMouseWheelScroll(sf::Event::MouseWheelScrolled wheelEvent)
{
    runLuaCallbacks(m_onMouseWheelScroll, wheelEvent.delta);
}
