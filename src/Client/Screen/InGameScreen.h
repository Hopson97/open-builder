#pragma once

#include "../../Maths.h"
#include "../GL/Shader.h"
#include "../GL/Texture.h"
#include "../GL/VertexArray.h"
#include "Screen.h"
#include <SFML/System/Clock.hpp>

#include <list>

class InGameScreen final : public Screen {
    struct Terrain {
        int index = 0;
        glm::vec3 location{0.0f};
        glpp::VertexArray vertexArray;
    };

  public:
    InGameScreen(ScreenManager& stack);
    ~InGameScreen();

    void onInput(const sf::Window& window, const Keyboard& keyboard) override;
    void onUpdate(float dt) override;
    void onRender() override;

  private:
    bool m_isPaused = false;
    bool m_isSettingsOpened = false;
    void showPauseMenu();

    glpp::Texture2d m_texture;
    glpp::VertexArray m_cubeVao;
    glpp::VertexArray m_terrainVao;
    glpp::Shader m_shader;
    glpp::UniformLocation m_locModelMat;
    glpp::UniformLocation m_locPvMat;
    glpp::UniformLocation m_locLightPos;
    glpp::UniformLocation m_locColour;
};