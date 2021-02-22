#include "ClientSettings.h"
#include "Screen/Screen.h"
#include <fstream>
#include <imgui/imgui.h>

ClientSettings::ClientSettings()
{
    std::ifstream inFile("Data/settings");
    std::string val;
    while (inFile >> val) {
        if (val == "show_fps") {
            inFile >> showFps;
        }
    }
}

ClientSettings::~ClientSettings()
{
    std::ofstream outFile("data/settings.txt");
    outFile << "show_fps" << ' ' << showFps << '\n';
}

void ClientSettings::showSettingsMenu(std::function<void(void)> onBackPressed)
{
    if (imguiBeginCustom("S E T T I N G S")) {
        ImGui::Separator();
        ImGui::Text("Basic Settings");
        ImGui::Checkbox("Show FPS Counter", &showFps);

        ImGui::Separator();

        ImGui::Separator();
        if (imguiButtonCustom("Back")) {
            onBackPressed();
        }
    }
    ImGui::End();
}
