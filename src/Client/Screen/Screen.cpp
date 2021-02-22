#include "Screen.h"

#include <imgui/imgui.h>

void ScreenManager::pushScreen(std::unique_ptr<Screen> screen)
{
    Action action;
    action.kind = Action::Kind::Push;
    action.screen = std::move(screen);
    m_actions.push_back(std::move(action));
}

void ScreenManager::popScreen()
{
    Action action;
    action.kind = Action::Kind::Pop;
    m_actions.push_back(std::move(action));
}

void ScreenManager::update()
{
    for (Action& action : m_actions) {
        switch (action.kind) {
            case Action::Kind::Push:
                m_screens.push(std::move(action.screen));
                break;

            case Action::Kind::Pop:
                m_screens.pop();
                break;
        }
    }
    m_actions.clear();
}

Screen& ScreenManager::peekScreen()
{
    return *m_screens.top();
}

bool ScreenManager::hasScreen() const
{
    return !m_screens.empty();
}

Screen::Screen(ScreenManager& screens)
    : m_pScreens(&screens)
{
}

bool imguiBeginCustom(const char* name)
{
    ImVec2 windowSize(1600 / 4, 900 / 2);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos({windowSize.x + windowSize.x * 4 / 8.0f, windowSize.y / 2},
                            ImGuiCond_Always);
    return ImGui::Begin(name, nullptr,
                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoCollapse |
                            ImGuiWindowFlags_AlwaysAutoResize);
}

bool imguiButtonCustom(const char* text)
{
    ImGui::SetCursorPos({ImGui::GetCursorPosX() + 100, ImGui::GetCursorPosY() + 20});
    return ImGui::Button(text, {100, 50});
}
