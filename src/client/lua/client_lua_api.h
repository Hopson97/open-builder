#pragma once

class ScriptEngine;

namespace gui {
class OverlayFactory;
}

class GuiRenderer;

void luaInitGuiApi(ScriptEngine& scriptEngine, gui::OverlayFactory& overlayFactory);
void luaIntGuiRenderApi(ScriptEngine& scriptEngine, GuiRenderer& guiRenderer);