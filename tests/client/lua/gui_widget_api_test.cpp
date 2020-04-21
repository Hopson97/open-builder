#include <catch2/catch.hpp>

#include <client/gui/gui_system.h>
#include <client/lua/client_lua_api.h>
#include <common/lua/script_engine.h>

const std::string guiCreateScript = R"(

    function create(overlay)
        local image = overlay:addImage()
        image.image = 52
    end

    game.gui():addGui{
        id = "test_gui",
        create = create
    }

    game.gui():change("test_gui")
)";

TEST_CASE("GUI Widget API Tests")
{
    ScriptEngine scriptEngine;
    gui::GuiSystem gui;
    luaInitGuiApi(scriptEngine, gui);
    luaInitGuiWidgetApi(scriptEngine);

    SECTION("GUIs can have widgets added to them by Lua scripts")
    {
        scriptEngine.runLuaString(guiCreateScript);
        gui.update();
        REQUIRE(gui.getActiveGuiRectCount() == 1);
    }
}
