#include "InGameScreen.h"

#include "../ClientSettings.h"
#include "../GL/GLDebug.h"
#include "../GL/Mesh.h"
#include "../Keyboard.h"
#include <SFML/Window/Keyboard.hpp>
#include <imgui/imgui.h>
#include <iostream>

struct Camera {
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0, 0, 0};
    glm::mat4 projectionMatrix{1.0f};

    void init(float aspectRatio, float fov);
    glm::mat4 getProjectionView() const;
} m_camera;

void Camera::init(float aspectRatio, float fov)
{
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, 0.5f, 2000.0f);
}

glm::mat4 Camera::getProjectionView() const
{
    return projectionMatrix * createViewMartix(position, rotation);
}

struct Player {
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f, 180.0f, 0.0f};
    glm::vec3 velocity{0.0f};
} m_player;

// Some variables I guess
auto projection = createProjectionMatrix(1280.0f / 720.0f, 75.0f);

InGameScreen::InGameScreen(ScreenManager& screens)
    : Screen(screens)
// , m_camera(1280.0f / 720.0f, 80)
{
    m_camera.init(1600 / 900, 75);

    // Create a shader
    m_shader.addShader("Static", glpp::ShaderType::Vertex);
    m_shader.addShader("Static", glpp::ShaderType::Fragment);
    m_shader.linkShaders();
    m_shader.bind();
    m_locModelMat = m_shader.getUniformLocation("modelMatrix");
    m_locPvMat = m_shader.getUniformLocation("projectionViewMatrix");
    m_locLightPos = m_shader.getUniformLocation("lightPosition");
    m_locColour = m_shader.getUniformLocation("colour");

    auto cube = glpp::createCubeMesh({1, 1, 1});
    m_cubeVao.bind();
    m_cubeVao.addAttribute(cube.positions, 3);
    m_cubeVao.addAttribute(cube.textureCoords, 2);
    m_cubeVao.addAttribute(cube.normals, 3);
    m_cubeVao.addElements(cube.indices);

    auto terrain = glpp::createTerrainMesh(0, {128, 128}, 1);
    m_terrainVao.bind();
    m_terrainVao.addAttribute(terrain.positions, 3);
    m_terrainVao.addAttribute(terrain.textureCoords, 2);
    m_terrainVao.addAttribute(terrain.normals, 3);
    m_terrainVao.addElements(terrain.indices);

    // Update lighting
    glpp::loadUniform(m_locLightPos, {0, 20, 0});

    m_texture.create("Player.png", true);
}

InGameScreen::~InGameScreen()
{
    glCheck(glUseProgram(0));
    glCheck(glBindVertexArray(0));
}

void InGameScreen::onInput(const sf::Window& window, const Keyboard& keyboard)
{
    if (!m_isPaused && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        m_isPaused = true;
    }
    if (m_isPaused) {
        return;
    }
    float SPEED = 0.5f;
    // Input for mouse
    static sf::Vector2i lastMousePosition = sf::Mouse::getPosition(window);
    sf::Vector2i change = sf::Mouse::getPosition(window) - lastMousePosition;
    m_player.rotation.x += static_cast<float>(change.y / 8.0f * 0.5);
    m_player.rotation.y += static_cast<float>(change.x / 8.0f * 0.5);
    sf::Mouse::setPosition({(int)window.getSize().x / 2, (int)window.getSize().y / 2});
    lastMousePosition = sf::Mouse::getPosition(window);

    // Inout for keyboard
    if (keyboard.isKeyDown(sf::Keyboard::W)) {
        m_player.velocity += forwardsVector(m_player.rotation) * SPEED;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::S)) {
        m_player.velocity += backwardsVector(m_player.rotation) * SPEED;
    }
    if (keyboard.isKeyDown(sf::Keyboard::A)) {
        m_player.velocity += leftVector(m_player.rotation) * SPEED;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::D)) {
        m_player.velocity += rightVector(m_player.rotation) * SPEED;
    }
}

void InGameScreen::onUpdate(float dt)
{
    if (m_isPaused) {
        return;
    }
    m_player.position += m_player.velocity * dt;
    m_player.velocity *= 0.9;

    // Update camera
    m_camera.position = m_player.position;
    m_camera.rotation = m_player.rotation;
}

void InGameScreen::onRender()
{
    glCheck(glEnable(GL_DEPTH_TEST));
    // Render GUI Stuff
    if (ClientSettings::get().showFps) {
    auto flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration |
                 ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                 ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (ImGui::Begin("FPS", nullptr, flags)) {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    }
    ImGui::End();
    }
    
    m_texture.bind();

    m_shader.bind();

    // Load up projection matrix stuff
    auto projectionView = m_camera.getProjectionView();
    auto modelmatrix = createModelMatrix({0, 0, 0}, {0, 0, 0});
    glpp::loadUniform(m_locModelMat, modelmatrix);
    glpp::loadUniform(m_locPvMat, projectionView);

    m_cubeVao.getDrawable().bind();
    m_cubeVao.getDrawable().draw();

    m_terrainVao.getDrawable().bind();
    m_terrainVao.getDrawable().draw();

    if (m_isPaused) {
        if (m_isSettingsOpened) {
            ClientSettings::get().showSettingsMenu([&] { m_isSettingsOpened = false; });
        }
        else {
            showPauseMenu();
        }
    }
    glCheck(glDisable(GL_DEPTH_TEST));

}

void InGameScreen::showPauseMenu()
{
    if (imguiBeginCustom("P A U S E D")) {
        if (imguiButtonCustom("Resume Game")) {
            m_isPaused = false;
        }
        if (imguiButtonCustom("Settings")) {
            m_isSettingsOpened = true;
        }
        if (imguiButtonCustom("Exit Game")) {
            m_pScreens->popScreen();
        }
    }
    ImGui::End();
}