#include "client_lua_api.h"

#include "../gui/widget.h"
#include "../gui/component.h"
#include <common/scripting/script_engine.h>

namespace {
    void addGuiDimensionApi(ScriptEngine& scriptEngine)
{
    auto gdimApi = scriptEngine.lua.new_usertype<gui::GuiDimension>(
        "GuiDim", sol::constructors<gui::GuiDimension(float, float, float, float)>());
}
void addGuiImageApi(ScriptEngine& engine)
{
    auto imageWidgetApi = engine.lua.new_usertype<gui::ImageWidget>("ImageWidget");
    imageWidgetApi["position"] = sol::property(&gui::ImageWidget::setPosition);
    imageWidgetApi["size"] = sol::property(&gui::ImageWidget::setSize);

    imageWidgetApi["image"] = sol::property(&gui::ImageWidget::setImage);
}
} // namespace

void luaInitGuiWidgetApi(ScriptEngine& scriptEngine)
{
    addGuiDimensionApi(scriptEngine);
    addGuiImageApi(scriptEngine);
}
