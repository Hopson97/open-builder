#include "Client/GL/GLDebug.h"
#include "Client/GL/Mesh.h"
#include "Client/GL/Shader.h"
#include "Client/GL/VertexArray.h"
#include "Client/Keyboard.h"
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

    // World world;
    //  Client client(world);
    // if (!client.connectTo(sf::IpAddress::getLocalAddress().toString())) {
    //     return -1;
    // }

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
    sf::Time lastTime = sf::Time::Zero;
    sf::Time lag = sf::Time::Zero;
    const int TPS = 30; // ticks per seconds
    const sf::Time timePerUpdate = sf::seconds(1.0f / float(TPS));
    int ticks = 0;

    // Create a shader
    glpp::Shader shader;
    shader.addShader("Static", glpp::ShaderType::Vertex);
    shader.addShader("Static", glpp::ShaderType::Fragment);
    shader.linkShaders();
    shader.bind();
    auto locModelMatrix = shader.getUniformLocation("modelMatrix");
    auto locPVMatrix = shader.getUniformLocation("projectionViewMatrix");

    // Create a vertex array
    std::vector<GLfloat> positions = {-0.5, -0.5, 0.0, 0.5,  -0.5, 0.0f,
                                      0.5,  0.5,  0.0, -0.5, 0.5,  0.0};
    //   std::vector<GLfloat> texCoords = {0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0};
    //   std::vector<GLfloat> normals = {0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.0f};
    std::vector<GLuint> indices = {0, 1, 2, 2, 3, 0};
    glpp::VertexArray quad;
    quad.bind();
    quad.addAttribute(positions, 3);
    // quad.addAttribute(texCoords, 2);
    // quad.addAttribute(normals, 3);
    quad.addElements(indices);

    // Some variables I guess
    glm::vec3 playerPosition{0.0f};
    glm::vec3 playerRotation{0.0f};
    glm::vec3 modelLocation{0.0, 0.0f, -1.0f};
    glm::vec3 modelRotation{0.0, 45.0f, 30.0f};
    auto projection = createProjectionMatrix(1280.0f / 720.0f, 75.0f);

    // Start the main game loop
    Keyboard keyboard;
    while (window.isOpen()) {
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
        // Update timestep variables
        auto time = timer.getElapsedTime();
        auto elapsed = time - lastTime;
        lastTime = time;
        lag += elapsed;
        const float PLAYER_SPEED = 0.01f;

        // Input stuff
        static auto lastMousePosition = sf::Mouse::getPosition(window);
        auto change = sf::Mouse::getPosition(window) - lastMousePosition;
        playerRotation.x += static_cast<float>(change.y / 8.0f * 0.5);
        playerRotation.y += static_cast<float>(change.x / 8.0f * 0.5);
        sf::Mouse::setPosition({(int)window.getSize().x / 2, (int)window.getSize().y / 2},
                               window);
        lastMousePosition.x = (int)window.getSize().x / 2;
        lastMousePosition.y = (int)window.getSize().y / 2;

        if (keyboard.isKeyDown(sf::Keyboard::W)) {
            playerPosition += forwardsVector(playerRotation) * PLAYER_SPEED;
        }
        else if (keyboard.isKeyDown(sf::Keyboard::S)) {
            playerPosition += backwardsVector(playerRotation) * PLAYER_SPEED;
        }
        if (keyboard.isKeyDown(sf::Keyboard::A)) {
            playerPosition += leftVector(playerRotation) * PLAYER_SPEED;
        }
        else if (keyboard.isKeyDown(sf::Keyboard::D)) {
            playerPosition += rightVector(playerRotation) * PLAYER_SPEED;
        }
        else if (keyboard.isKeyDown(sf::Keyboard::Escape)) {
            window.close();
        }

        // Update
        // Fixed time update
        while (lag >= timePerUpdate) {
            ticks++;
            lag -= timePerUpdate;
            // update stuff
        }

        // Render...
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render GUI
        // ImGui_SfGl::startFrame();
        //   ImGui::ShowDemoWindow();
        // ImGui_SfGl::endFrame();

        // Draw stuff
        auto modelmatrix = createModelMatrix(modelLocation, modelRotation);
        auto viewmatrix = createViewMartix(playerPosition, playerRotation);
        glpp::loadUniform(locModelMatrix, modelmatrix);
        glpp::loadUniform(locPVMatrix, projection * viewmatrix);

        quad.getDrawable().bind();
        quad.getDrawable().draw();

        // Render other stuff

        window.display();
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