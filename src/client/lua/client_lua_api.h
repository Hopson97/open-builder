#pragma once

class ScriptEngine;

#include <optional>

namespace gui {
class OverlayFactory;
}

class GuiRenderer;

void luaInitGuiApi(ScriptEngine& scriptEngine, gui::OverlayFactory& overlayFactory,
                   GuiRenderer* guiRenderer);