#include "client_lua_api.h"

#include <common/scripting/script_engine.h>

#include "../gui/overlay.h"
#include "../gui/overlay_factory.h"
#include "../renderer/gui_renderer.h"

namespace {
void initOverlayDefinitionCreateApi(sol::table& guiTable,
                                    gui::OverlayFactory& overlayFactory)
{
    guiTable["defineGui"] = [&](const sol::table& guiDefintion) {
        gui::OverlayDefinition overlay;

        overlay.id = guiDefintion["id"];
        overlay.title = guiDefintion["title"];
        overlay.create = guiDefintion["create"];

        overlayFactory.addOverlay(overlay);
    };
}

} // namespace

void luaInitGuiApi(ScriptEngine& scriptEngine, gui::OverlayFactory& overlayFactory)
{
    auto guiTable = scriptEngine.addTable("gui");
    initOverlayDefinitionCreateApi(guiTable, overlayFactory);
}

void luaIntGuiRenderApi(ScriptEngine& scriptEngine, GuiRenderer& guiRenderer)
{
    scriptEngine.lua["game"]["gui"]["getTexture"] =
        [&guiRenderer](const std::string& path) { return guiRenderer.getTexture(path); };
}

/*
#include "../gui/gui_container.h"
#include "../gui/gui_master.h"
#include "../gui/gui_rectangle.h"
#include <common/scripting/script_engine.h>

namespace {
void addGuiMasterApi(ScriptEngine& scriptEngine, GuiMaster& guiMaster)
{
    auto gui = scriptEngine.addTable("gui");

    gui["getTexture"] = [&guiMaster](const std::string& path) {
        return guiMaster.getTexture(path);
    };

    gui["makeGui"] = [&guiMaster]() { return guiMaster.addGui(); };
}

void addGuiContainerApi(ScriptEngine& scriptEngine)
{
    auto containerApi = scriptEngine.lua.new_usertype<GuiContainer>("GuiContainer");

    containerApi["show"] = &GuiContainer::show;
    containerApi["hide"] = &GuiContainer::hide;
    containerApi["addRect"] = &GuiContainer::addRectangle;
    containerApi["addText"] = &GuiContainer::addText;
}

void addGuiDimensionApi(ScriptEngine& scriptEngine)
{
    auto gdimApi = scriptEngine.lua.new_usertype<GuiDimension>(
        "GuiDim", sol::constructors<GuiDimension(float, float, float, float)>());
}

void addGuiRectangleApi(ScriptEngine& scriptEngine)
{
    auto rectangleApi = scriptEngine.lua.new_usertype<GuiRectangle>("GuiRectangle");

    rectangleApi["show"] = &GuiRectangle::show;
    rectangleApi["hide"] = &GuiRectangle::hide;
    rectangleApi["position"] = sol::property(&GuiRectangle::setPosition);
    rectangleApi["size"] = sol::property(&GuiRectangle::setSize);
    rectangleApi["texture"] = sol::property(&GuiRectangle::setTexture);
    rectangleApi["colour"] = sol::property(&GuiRectangle::setColour);
}

void addGuiTextApi(ScriptEngine& scriptEngine)
{
    auto textApi = scriptEngine.lua.new_usertype<GuiText>("GuiText");

    textApi["show"] = &GuiText::show;
    textApi["hide"] = &GuiText::hide;
    textApi["position"] = sol::property(&GuiText::setPosition);
    textApi["text"] = sol::property(&GuiText::setText);
    textApi["size"] = sol::property(&GuiText::setFontSize);
}

} // namespace

void initGuiApi(ScriptEngine& scriptEngine, GuiMaster& guiMaster)
{
    addGuiMasterApi(scriptEngine, guiMaster);
    addGuiContainerApi(scriptEngine);
    addGuiDimensionApi(scriptEngine);
    addGuiRectangleApi(scriptEngine);
    addGuiTextApi(scriptEngine);
}

*/

/*
struct GuiScreen {
    std::string name;
    sol::function onCreate;
};

namespace {
void initDefineGuiApi(ScriptEngine& scriptEngine, GuiMaster& guiMaster)
{
    auto gui = scriptEngine.addTable("gui");
    gui["defineGui"] = [&](sol::table guiTable) {
        GuiScreen screen;
        screen.name = guiTable.get<std::string>("name");
        screen.onCreate = guiTable.get<sol::function>("onCreate");

        // add thing to master idk
    };
}
} // namespace

void luaInitGuiApi(ScriptEngine& scriptEngine, GuiMaster& guiMaster)
{
    initDefineGuiApi(scriptEngine, guiMaster);
}
*/