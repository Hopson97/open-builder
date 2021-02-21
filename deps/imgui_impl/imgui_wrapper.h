#pragma once

// Quick and dirty wrapper for SFML + OpenGL 3 + ImGui

#include <SFML/Window/Window.hpp>
#include "../imgui/imgui.h"
#include "imgui_impl_sfml.h"
#include "imgui_impl_opengl3.h"

namespace ImGui_SfGl {
    void init(sf::Window& window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplSfml_Init(&window);
        ImGui_ImplOpenGL3_Init();
    }

    void startFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSfml_NewFrame();
        ImGui::NewFrame();
    }

    void endFrame() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void shutdown() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSfml_Shutdown();
        ImGui::DestroyContext();
    }
} // namespace ImGuiSfGl