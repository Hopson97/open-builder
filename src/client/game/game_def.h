#pragma once

#include "../network/client.h"
#include <SFML/Window/Event.hpp>

#include "../gl/shader.h"
#include "../gl/vertex_array.h"
#include "../renderer/camera.h"
#include "../gl/textures.h"

class Keyboard;
struct InputState;

void handleFpsCameraInput(glm::vec3& rotation, glm::vec3& velocity,
                          const Keyboard& keyboard, const InputState& inputState);

class ClientGameDef {
  public:
    virtual ~ClientGameDef() = default;

    void handleEvent(const sf::Event& event);
    void handleInput(const Keyboard& keyboard, const InputState& inputState);
    void tick(float dt);
    void render();

    virtual bool start() = 0;

    void shutdown();

  protected:
    bool start(const std::string ipAddress);

  private:
    virtual void onShutdown() = 0;

    // temp
    gl::VertexArray m_vao;
    gl::Shader m_entityShader;
    gl::UniformLocation m_entityProj;
    gl::UniformLocation m_entityModel;
    gl::Texture2d m_playerTexture;

    Camera m_camera;

    Client m_client;

    EntityState m_playerPosition;
    glm::vec3 m_playerVelocity{0.0f};
};