#pragma once

#include <SFML/System/Vector3.hpp>
#include <common/types.h>

namespace client {
    using BlockId = u8;

    enum class BlockType : BlockId {
        Air,
        Grass,
    };

    struct Block {
        Block() = default;

        Block(BlockType blockType)
            : type(blockType)
        {
        }

        BlockType type = BlockType::Grass;
    };
} // namespace client
