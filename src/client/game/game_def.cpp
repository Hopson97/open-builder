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


ClientGameDef::ClientGameDef()
    : m_client(m_world, m_player)
{
}

bool ClientGameDef::start(const std::string ipAddress)
{
    auto connection = m_client.connectTo(ipAddress);
    if (!connection.success) {
        std::cout << "ERROR: " << connection.message << "\n";
        shutdown();
        return false;
    }
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
        m_player.input(keyboard);
    }

    // Test whether the voxel the player is looking at is interactable
    auto& position = m_player.m_state.position;
    auto& rotation = m_player.m_state.rotation;

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

    m_camera.update(m_player.m_state);
    m_world.tick(dt);
    if (m_client.getConnnectionState() == ConnectionState::Disconnected) {
        shutdown();
    }

    m_player.tick(m_world, dt);

    auto thisTime = m_timer.getElapsedTime();
    if (thisTime - m_lastTime > sf::milliseconds(50)) {
        m_client.sendPlayerState(m_player.m_state);
    }
}

void ClientGameDef::render()
{
    m_world.render(m_camera);
    if (m_isVoxelSelected) {
        m_selectionBoxRenderer.render(m_camera, m_currentSelectedVoxelPos);
    }
}
