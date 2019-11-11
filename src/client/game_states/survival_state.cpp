#include "survival_state.h"

#include <glad/glad.h>
#include <iostream>

#include "../geometry/geometry_factory.h"
#include "../input/keyboard.h"
#include "../renderer/camera.h"
#include "../renderer/renderer.h"
#include "state_handler.h"

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>

namespace client {
    SurvivalState::SurvivalState(StateHandler &stateHandler)
        : GameState(stateHandler)
        , m_client(m_entities)
    {
        if (!m_client.connect(sf::IpAddress::LocalHost)) {
            mp_stateHandler->popState();
        }

        for (auto &entity : m_entities) {
            entity.alive = false;
            entity.model = &m_defaultModel;
        }

        for (int i = 0; i < (int)m_client.getMaxPlayers(); i++) {
            m_entities[i].model = &m_defaultModel;
        }

        auto &c = m_chunks.emplace_back();
        ChunkMeshBuilder builder(c);
        m_chunkMeshes.push_back(builder.createMesh());
    }

    void SurvivalState::handleKeyUp(sf::Keyboard::Key key)
    {
        if (key == sf::Keyboard::L) {
            m_isMouseLocked = !m_isMouseLocked;
        }
    }

    void SurvivalState::handleInput(const Keyboard &keyboard,
                                    const sf::Window &window)
    {
        // Keyboard input
        m_inputState = 0;
        if (keyboard.isKeyDown(sf::Keyboard::Key::W)) {
            m_inputState |= PlayerInput::Forwards;
        }
        else if (keyboard.isKeyDown(sf::Keyboard::Key::S)) {
            m_inputState |= PlayerInput::Back;
        }
        if (keyboard.isKeyDown(sf::Keyboard::Key::D)) {
            m_inputState |= PlayerInput::Right;
        }
        else if (keyboard.isKeyDown(sf::Keyboard::Key::A)) {
            m_inputState |= PlayerInput::Left;
        }

        // Mouse input
        if (!m_isMouseLocked) {
            static float const BOUND = 89.f;
            static auto lastMousePosition = sf::Mouse::getPosition(window);
            auto change = sf::Mouse::getPosition() - lastMousePosition;
            auto &rotation = getPlayerEntity().transform.rotation;

            rotation.y += change.x * 0.05f;
            rotation.x += change.y * 0.05f;

            if (rotation.x > BOUND)
                rotation.x = BOUND;
            else if (rotation.x < -BOUND)
                rotation.x = -BOUND;

            if (rotation.y > 360)
                rotation.y = 0;
            else if (rotation.y < 0)
                rotation.y = 360;

            sf::Mouse::setPosition({static_cast<int>(window.getSize().x / 2),
                                    static_cast<int>(window.getSize().y / 2)},
                                   window);

            lastMousePosition = sf::Mouse::getPosition();
        }

        m_client.sendInput(m_inputState, getPlayerEntity().transform.rotation);
    }

    void SurvivalState::update(Camera &camera)
    {
        m_client.update();
        camera.reset(getPlayerEntity().transform);
    }

    void SurvivalState::render(Renderer &renderer)
    {
        unsigned i = 0;
        for (const auto &e : m_entities) {
            if (e.alive && i++ != m_client.getClientId()) {
                renderer.process(e);
            }
        }

        for (auto &chunk : m_chunkMeshes) {
            renderer.process(chunk);
        }
    }

    void SurvivalState::onExit()
    {
        if (m_client.isConnected()) {
            m_client.disconnect();
        }
    }

    Entity &SurvivalState::getPlayerEntity()
    {
        return m_entities[m_client.getClientId()];
    }
} // namespace client
