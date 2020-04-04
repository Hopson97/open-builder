#include "game_def.h"

#include "../gl/primitive.h"

bool ClientGameDef::start(const std::string ipAddress)
{
    m_vao = makeCubeVertexArray(1, 2, 1);
    m_camera = Camera::createCamera({0, 0, 0});

    m_entityShader.create("static", "static");
    m_entityShader.bind();
    m_playerTexture.create("res/skins/player.png", false);

    m_entityProj = m_entityShader.getUniformLocation("projectionViewMatrix");
    m_entityModel = m_entityShader.getUniformLocation("modelMatrix");

    auto connection = m_client.connectTo(ipAddress);
    if (!connection.success) {
        std::cout << "ERROR: " << connection.message << "\n";
        shutdown();
        return false;
    }
    return true;
}

void ClientGameDef::handleEvent(const sf::Event& event)
{
}

void ClientGameDef::handleInput(const Keyboard& keybaord, const InputState& inputState)
{
}

void ClientGameDef::tick(float dt)
{
    m_camera.m_frustum.update(m_camera.m_projection);
    m_client.tick();
    if (m_client.getConnnectionState() == ConnectionState::Disconnected) {
        shutdown();
    }
}

void ClientGameDef::render()
{
    m_entityShader.bind();
    m_playerTexture.bind();
    auto pv = createProjectionViewMatrix(
        m_camera.position, m_camera.rotation, m_camera.m_projection);

    m_camera.m_frustum.update(pv);

    glm::mat4 model{1.0f};
    translateMatrix(model, {0, 0, -2});
    gl::loadUniform(m_entityProj, pv);
    gl::loadUniform(m_entityModel, model);

    m_vao.getDrawable().bindAndDraw();
}

void ClientGameDef::shutdown()
{
    m_client.disconnect();
    onShutdown();
}