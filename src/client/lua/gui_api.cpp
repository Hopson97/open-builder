#include "client_lua_api.h"

#include <common/lua/script_engine.h>

#include "../gui/gui_system.h"
#include "../renderer/gui_renderer.h"

namespace {
    using Gui = gui::GuiSystem;

    void initGuiRenderApi(sol::table& guiTable, GuiRenderer& guiRenderer)
    {
        guiTable["getTexture"] = [&guiRenderer](const std::string& path) {
            return guiRenderer.getTexture(path);
        };
    }
} // namespace

void luaInitGuiApi(ScriptEngine& scriptEngine, gui::GuiSystem& guiSystem,
                   GuiRenderer* guiRenderer)
{
    scriptEngine.gameTable["gui"] = [&guiSystem]() -> gui::GuiSystem& {
        return guiSystem;
    };

    auto guiApi = scriptEngine.lua.new_usertype<gui::GuiSystem>("GUISystem");
    guiApi["push"] = &Gui::pushGui;
    guiApi["pop"] = &Gui::popGui;
    guiApi["change"] = &Gui::changeGui;
    guiApi["addGui"] = [](Gui& guiSystem, const sol::table& guiDefintion) {
        gui::OverlayDefinition overlay;

        overlay.id = guiDefintion["id"];
        overlay.create = guiDefintion["create"];

        guiSystem.addGuiDefintion(overlay);
    };

    if (guiRenderer) {
        initGuiRenderApi(scriptEngine.gameTable, *guiRenderer);
    }
}
