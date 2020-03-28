#include "client_lua_api.h"

#include <common/lua/script_engine.h>

#include "../gui/gui_system.h"
#include "../renderer/gui_renderer.h"

void luaInitGuiApi(ScriptEngine& scriptEngine, gui::GuiSystem& guiSystem,
                   GuiRenderer* guiRenderer)
{
    using Gui = gui::GuiSystem;

    scriptEngine.gameTable["gui"] = [&guiSystem]() -> gui::GuiSystem& {
        return guiSystem;
    };

    auto guiApi = scriptEngine.lua.new_usertype<Gui>("GUISystem");
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
        scriptEngine.gameTable["getTexture"] = [guiRenderer](const std::string& path) {
            return guiRenderer->getTexture(path);
        };
    }
}
