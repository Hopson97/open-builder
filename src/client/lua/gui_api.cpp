#include "client_lua_api.h"

#include "../gui/gui_container.h"
#include "../gui/gui_master.h"
#include "../gui/gui_rectangle.h"
#include <common/scripting/script_engine.h>

namespace {
void addGuiMasterApi(ScriptEngine& engine, GuiMaster& guiMaster)
{
    auto gui = engine.addTable("gui");

    gui["getTexture"] = [&guiMaster](const std::string& path) {
        return guiMaster.getTexture(path);
    };

    gui["add"] = [&guiMaster](GuiContainer& container) { guiMaster.addGui(container); };
}

void addGuiContainerApi(ScriptEngine& engine)
{
    auto containerApi = engine.lua.new_usertype<GuiContainer>(
        "GuiContainer", sol::constructors<GuiContainer>());

    containerApi["show"] = &GuiContainer::hide;
    containerApi["hide"] = &GuiContainer::hide;
    containerApi["add"] = &GuiContainer::addRectangle;
}

void addGuiDimensionApi(ScriptEngine& engine)
{
    auto gdimApi = engine.lua.new_usertype<GuiDimension>(
        "GuiDim", sol::constructors<GuiDimension(float, float, float, float)>());
}

void addGuiRectangleApi(ScriptEngine& engine)
{
    auto rectangleApi = engine.lua.new_usertype<GuiRectangle>(
        "GuiRectangle",
        sol::constructors<GuiRectangle(),
                          GuiRectangle(const GuiDimension&, const GuiDimension&)>());

    rectangleApi["position"] = sol::property(&GuiRectangle::setPosition);
    rectangleApi["size"] = sol::property(&GuiRectangle::setSize);
}

} // namespace

void initGuiApi(ScriptEngine& engine, GuiMaster& guiMaster)
{
    addGuiMasterApi(engine, guiMaster);
    addGuiContainerApi(engine);
    addGuiDimensionApi(engine);
    addGuiRectangleApi(engine);
}