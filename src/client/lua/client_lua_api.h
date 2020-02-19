#pragma once

class ScriptEngine;
class GuiMaster;
struct ClientLuaCallbacks;

void initGuiApi(ScriptEngine& engine, GuiMaster& guiMaster);
void initClientCallbackApi(ScriptEngine& engine, ClientLuaCallbacks& callbacks);