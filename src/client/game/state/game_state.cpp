#include "state_handler.h"

namespace client {

    GameState::GameState(StateHandler &stateHandler)
        : mp_stateHandler(&stateHandler)
    {
    }
} // namespace client
