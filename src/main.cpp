#include "Client/ClientSettings.h"
#include "Client/GL/GLDebug.h"
#include "Client/GL/Mesh.h"
#include "Client/GL/Shader.h"
#include "Client/GL/VertexArray.h"
#include "Client/Keyboard.h"
#include "Client/Screen/MainMenuScreen.h"
#include "Client/Screen/Screen.h"
#include "Maths.h"
#include <SFML/GpuPreference.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <atomic>
#include <enet/enet.h>
#include <glad/glad.h>
#include <imgui/imgui.h>
#include <imgui_impl/imgui_wrapper.h>
#include <iostream>
#include <thread>
#include <vector>

SFML_DEFINE_DISCRETE_GPU_PREFERENCE

namespace {
    const int TPS = 30; // ticks per seconds
    const sf::Time timePerUpdate = sf::seconds(1.0f / float(TPS));

    void showFPS()
    {
        if (ClientSettings::get().showFps) {
            auto flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration |
                         ImGuiWindowFlags_AlwaysAutoResize |
                         ImGuiWindowFlags_NoSavedSettings |
                         ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
            if (ImGui::Begin("FPS", nullptr, flags)) {
                ImGuiIO& io = ImGui::GetIO();
                ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate,
                            io.Framerate);
            }
            ImGui::End();
        }
    }
} // namespace

// ================================================
//
//          S E R V E R   M A I N
//
// =================================================
int serverMain()
{
    std::atomic_bool isRunning{true};

    std::thread console([&]() {
        std::string line;
        while (isRunning) {
            std::cout << "Type a command.\n> ";
            std::getline(std::cin, line);
            if (line == "exit") {
                isRunning = false;
            }
        }
    });

    std::cout << "Starting server.\n";
    // World world;
    // Server server(world);
    while (isRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        //    server.tick();
    }

    console.join();
    return 0;
}

// ================================================
//
//          C L I E N T   M A I N
//
// =================================================
int clientMain()
{
    // Set up the window
    sf::Window window;
    {
        sf::ContextSettings contextSettings;
        contextSettings.depthBits = 24;
        contextSettings.stencilBits = 8;
        contextSettings.antialiasingLevel = 4;
        contextSettings.majorVersion = 3;
        contextSettings.minorVersion = 3;
        contextSettings.attributeFlags = sf::ContextSettings::Core;
        window.create({1600, 900}, "open-builder", sf::Style::Close, contextSettings);
        window.setPosition(
            {(int)sf::VideoMode::getDesktopMode().width / 2 - (int)window.getSize().x / 2,
             0});

        window.setFramerateLimit(60);
        window.setKeyRepeatEnabled(false);
        if (!gladLoadGL()) {
            std::cerr << "Failed to load OpenGL, exiting." << std::endl;
            return 1;
        }
        initGLDebug();
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glViewport(0, 0, window.getSize().x, window.getSize().y);
    }
    // Start imgui
    ImGui_SfGl::init(window);
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 2;
        style.FrameRounding = 0;
        style.PopupRounding = 0;
        style.ScrollbarRounding = 0;
        style.TabRounding = 6;
    }

    // Timestep variables
    sf::Clock timer;
    sf::Clock dt;
    sf::Time lastTime = sf::Time::Zero;
    sf::Time lag = sf::Time::Zero;

    // Start the screens
    ScreenManager screens;
    screens.pushScreen(std::make_unique<MainMenuScreen>(screens));
    screens.update();

    // Start the main game loop
    Keyboard keyboard;
    while (window.isOpen() && screens.hasScreen()) {
        // Handle window events...
        sf::Event e;
        while (window.pollEvent(e)) {
            ImGui_ImplSfml_ProcessEvent(e);
            keyboard.update(e);
            switch (e.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                default:
                    break;
            }
        }
        Screen& screen = screens.peekScreen();
        screen.onInput(window, keyboard);

        sf::Time time = timer.getElapsedTime();
        sf::Time elapsed = time - lastTime;
        lastTime = time;
        lag += elapsed;
        while (lag >= timePerUpdate) {
            lag -= timePerUpdate;
            screen.onUpdate(dt.restart().asSeconds());
        }

        // Render...
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_SfGl::startFrame();
        showFPS();

        screen.onRender();
        ImGui_SfGl::endFrame();

        window.display();
        screens.update();
    }
    ImGui_SfGl::shutdown();
    return 0;
}

// ================================================
//
//                   M A I N
//
// =================================================
int main(int argc, char** argv)
{
    // Init ENET library and connect the client
    if (enet_initialize() != 0) {
        return EXIT_FAILURE;
    }

    // Convert command line into std::string
    std::vector<std::string> args;
    for (int i = 1; i < argc; i++) {
        if (argc > 1) {
            std::cout << args.emplace_back(argv[i]) << std::endl;
        }
    }

    if (args.size() > 0) {
        if (args[0] == "client") {
            return clientMain();
        }
        else if (args[0] == "server") {
            return serverMain();
        }
    }
    else {
        //  std::thread t([]() { serverMain(); });
        //  std::this_thread::sleep_for(std::chrono::milliseconds(100));
        clientMain();
        //  t.join();
    }

    enet_deinitialize();
}