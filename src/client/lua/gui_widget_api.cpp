#include "client_lua_api.h"

#include "../gui/component.h"
#include "../gui/overlay.h"
#include "../gui/widget.h"
#include <common/scripting/script_engine.h>

namespace {

template <typename T>
void addCommonAPI(sol::usertype<T>& api)
{
    api["position"] = sol::property(&T::setPosition);
    api["size"] = sol::property(&T::setSize);
}

void addGuiDimensionApi(ScriptEngine& scriptEngine)
{
    auto gdimApi = scriptEngine.lua.new_usertype<gui::GuiDimension>(
        "GuiDim", sol::constructors<gui::GuiDimension(float, float, float, float)>());
}

void addGuiImageApi(ScriptEngine& engine)
{
    auto imageWidgetApi = engine.lua.new_usertype<gui::ImageWidget>("ImageWidget");
    imageWidgetApi["image"] = sol::property(&gui::ImageWidget::setImage);

    addCommonAPI(imageWidgetApi);
}

void addGuiLabelApi(ScriptEngine& engine)
{
    auto labelApi = engine.lua.new_usertype<gui::LabelWidget>("LabelWidget");
    labelApi["text"] = sol::property(&gui::LabelWidget::setText);
    labelApi["textSize"] = sol::property(&gui::LabelWidget::setTextSize);

    addCommonAPI(labelApi);
}

void initGuiOverlayApi(ScriptEngine& engine)
{
    auto overlayApi = engine.lua.new_usertype<gui::Overlay>("GuiOverlay");
    overlayApi["addImage"] = &gui::Overlay::addImage;
    overlayApi["addLabel"] = &gui::Overlay::addLabel;
}

} // namespace

void luaInitGuiWidgetApi(ScriptEngine& scriptEngine)
{
    addGuiDimensionApi(scriptEngine);
    addGuiImageApi(scriptEngine);
    initGuiOverlayApi(scriptEngine);
    addGuiLabelApi(scriptEngine);
}
