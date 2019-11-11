#pragma once

#include "game_state.h"

#include <common/network/input_state.h>
#include <common/types.h>

#include <vector>
#include <unordered_map>

#include "../world/chunk/chunk.h"
#include "../world/chunk/mesh/chunk_mesh_builder.h"

#include "../network/client.h"

#include "../world/world.h"

namespace client {
    class StateHandler;
    class Keyboard;
    class Renderer;

    /// The survival state is the actual "playing" state of the game
    /// This will play by connecting to a server
    class SurvivalState final : public GameState {
      public:
        SurvivalState(StateHandler &stateHandler);

        void handleKeyUp(sf::Keyboard::Key key) override;

        void handleInput(const Keyboard &keyboard,
                         const sf::Window &window) override;
        void update(Camera &camera) override;
        void render(Renderer &renderer) override;

        void onExit() override;

      private:
        Entity &getPlayerEntity();

        Client m_client;
        Input m_inputState = 0;

        TexturedModel m_defaultModel;
        TexturedModel m_playerModel;

        World m_world;

        bool m_isMouseLocked = false;
    };
} // namespace client
