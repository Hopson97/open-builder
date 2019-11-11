#pragma once

#include <array>
#include <common/network/constants.h>
#include <common/types.h>
#include <common/util/maths.h>

#include "../renderer/model/textured_model.h"

namespace client {

    struct Entity {
        maths::Transform transform;
        TexturedModel *model;
        u8 alive = 0;
    };

    using EntityArray = std::array<Entity, ENTITY_COUNT>;
} // namespace client
