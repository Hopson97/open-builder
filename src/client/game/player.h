#pragma once

#include <common/world/entity_state.h>
#include <common/types.h>

class Keyboard;
class ClientWorld;

class Player {
  public:
    void input(const Keyboard& keyboard);
    void tick(ClientWorld& world, float dt);

    EntityState m_state;
    u32 playerId;

  private:
    void collide(const ClientWorld& world, const glm::vec3& vel);

    void keyboardInput(const Keyboard& keyboard);
    void mouseInput();

    glm::vec3 m_acceleration;

    bool m_isOnGround = false;
    bool m_isFlying = false;
};