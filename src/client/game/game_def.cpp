#include "game_def.h"

#include "../client_config.h"
#include "../gl/gl_errors.h"
#include "../gl/primitive.h"
#include "../input/input_state.h"
#include "../input/keyboard.h"
#include "../window.h"

void SelectedBoxRenderer::create()
{
    program.create("selection", "selection");
    program.bind();
    modelLocation = program.getUniformLocation("modelMatrix");
    projectionViewLocation = program.getUniformLocation("projectionViewMatrix");
    m_selectionBox = makeWireCubeVertexArray(1.02f, 1.02f, 1.02f);
}

void SelectedBoxRenderer::render(const Camera& camera, const VoxelPosition& position)
{
    glCheck(glEnable(GL_BLEND));
    glCheck(glEnable(GL_LINE_SMOOTH));
    glCheck(glLineWidth(2.0));
    program.bind();
    glm::mat4 modelMatrix{1.0};
    float size = 1.005f;
    translateMatrix(modelMatrix,
                    {position.x - (size - 1) / 2, position.y - (size - 1) / 2,
                     position.z - (size - 1) / 2});
    scaleMatrix(modelMatrix, size);
    gl::loadUniform(modelLocation, modelMatrix);
    gl::loadUniform(projectionViewLocation, camera.getProjectionView());
    m_selectionBox.getDrawable().bindAndDraw(GL_LINES);
    glCheck(glDisable(GL_BLEND));
}

bool ClientGameDef::start(const std::string ipAddress)
{
    auto connection = m_client.connectTo(ipAddress);
    if (!connection.success) {
        std::cout << "ERROR: " << connection.message << "\n";
        shutdown();
        return false;
    }
    m_client.setWorld(m_world);

    m_camera = Camera::createCamera();
    m_selectionBoxRenderer.create();

    // TODO Move to the client/server handling
    m_world.setupData(1024);
    return true;
}

void ClientGameDef::shutdown()
{
    m_client.disconnect();
    onShutdown();
}

void ClientGameDef::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            m_client.sendMouseEvent(MouseEventState::Click);
        }
        else {
            m_client.sendInteraction();
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            m_client.sendMouseEvent(MouseEventState::Release);
        }
    }
}

void ClientGameDef::handleInput(const Keyboard& keyboard, const InputState& inputState)
{
    if (m_client.getConnnectionState() != ConnectionState::Connected) {
        return;
    }

    if (inputState.isMouseLocked) {
        handlePlayerInput(keyboard);
    }

    // Test whether the voxel the player is looking at is interactable
    auto& player = m_world.getPlayer();
    auto& position = player.position;
    auto& rotation = player.rotation;

    m_isVoxelSelected = false;
    auto voxels = getIntersectedVoxels(position, forwardsVector(rotation), 8);
    for (auto& position : voxels) {
        if (m_world.isVoxelInteractable(position)) {
            m_currentSelectedVoxelPos = position;
            m_isVoxelSelected = true;
            break;
        }
    }
}

void ClientGameDef::tick(float dt)
{
    m_client.tick();
    if (m_client.getConnnectionState() != ConnectionState::Connected) {
        return;
    }

    m_camera.update(m_world.getPlayer());
    m_world.tick(dt);
    if (m_client.getConnnectionState() == ConnectionState::Disconnected) {
        shutdown();
    }

    auto thisTime = m_timer.getElapsedTime();
    if (thisTime - m_lastTime > sf::milliseconds(50)) {
        m_client.sendPlayerState(m_world.getPlayer());
    }
}

void ClientGameDef::render()
{
    m_world.render(m_camera);
    if (m_isVoxelSelected) {
        m_selectionBoxRenderer.render(m_camera, m_currentSelectedVoxelPos);
    }
}

void ClientGameDef::handlePlayerInput(const Keyboard& keyboard)
{
    auto& ctx = *Window::context;
    static auto lastMousePosition = sf::Mouse::getPosition(ctx);

    auto& player = m_world.getPlayer();
    glm::vec3& rotation = player.rotation;
    glm::vec3& velocity = player.velocity;

    float verticalSensitivity = ClientConfig::get().verticalSensitivity;
    float horizontalSensitivity = ClientConfig::get().horizontalSensitivity;
    auto change = sf::Mouse::getPosition(ctx) - lastMousePosition;
    rotation.x += static_cast<float>(change.y / 8.0f * verticalSensitivity);
    rotation.y += static_cast<float>(change.x / 8.0f * horizontalSensitivity);
    sf::Mouse::setPosition({(int)ctx.getSize().x / 2, (int)ctx.getSize().y / 2}, ctx);

// This fixes mouse jittering on mac
#ifndef __APPLE__
    lastMousePosition = sf::Mouse::getPosition(ctx);
#else
    lastMousePosition.x = (int)window.getSize().x / 2;
    lastMousePosition.y = (int)window.getSize().y / 2;
#endif

    float PLAYER_SPEED = 3.0f;
    if (keyboard.isKeyDown(sf::Keyboard::LControl)) {
        PLAYER_SPEED *= 10;
    }

    if (keyboard.isKeyDown(sf::Keyboard::W)) {
        velocity += forwardsVector(rotation) * PLAYER_SPEED;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::S)) {
        velocity += backwardsVector(rotation) * PLAYER_SPEED;
    }
    if (keyboard.isKeyDown(sf::Keyboard::A)) {
        velocity += leftVector(rotation) * PLAYER_SPEED;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::D)) {
        velocity += rightVector(rotation) * PLAYER_SPEED;
    }
    if (keyboard.isKeyDown(sf::Keyboard::Space)) {
        velocity.y += PLAYER_SPEED * 2;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::LShift)) {
        velocity.y -= PLAYER_SPEED * 2;
    }
    if (rotation.x < -80.0f) {
        rotation.x = -79.9f;
    }
    else if (rotation.x > 85.0f) {
        rotation.x = 84.9f;
    }
}
