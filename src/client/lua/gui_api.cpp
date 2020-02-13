#include "client_lua_api.h"

#include "../gui/gui_container.h"
#include "../gui/gui_master.h"
#include <common/scripting/script_engine.h>

namespace {
void addGuiContainerApi(ScriptEngine& engine)
{
    auto containerApi = engine.lua.new_usertype<GuiContainer>(
        "GuiContainer", sol::constructors<GuiContainer>());

    containerApi["show"] = &GuiContainer::hide;
    containerApi["hide"] = &GuiContainer::hide;
}

void addGuiMasterApi(ScriptEngine& engine, GuiMaster& guiMaster)
{
    auto gui = engine.addTable("gui");

    gui["getTexture"] = [&guiMaster](const std::string& path) {
        return guiMaster.getTexture(path);
    };

    gui["add"] = [&guiMaster](GuiContainer& container) { guiMaster.addGui(container); };
}
} // namespace

void initGuiApi(ScriptEngine& engine, GuiMaster& guiMaster)
{
    addGuiMasterApi(engine, guiMaster);
    addGuiContainerApi(engine);
}