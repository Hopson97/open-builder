#pragma once

#include "../types.h"
#include <SFML/System/Vector3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>

using ChunkPosition = sf::Vector3<i32>;
using BlockPosition = sf::Vector3<i32>;

struct ChunkPositionHash {
    // http://www.beosil.com/download/CollisionDetectionHashing_VMV03.pdf
    std::size_t operator()(const ChunkPosition &position) const
    {
        return (position.x * 88339) ^ (position.z * 91967) ^
               (position.z * 126323);
    }
};

template <typename T>
using ChunkPositionMap =
    std::unordered_map<ChunkPosition, T, ChunkPositionHash>;

int toLocalBlockIndex(const BlockPosition &position);

ChunkPosition toChunkPosition(const BlockPosition &position);
BlockPosition toLocalBlockPosition(const BlockPosition &position);
BlockPosition toGlobalBlockPosition(const BlockPosition &blockPosition,
                                    const ChunkPosition &localChunkPosition);
BlockPosition toBlockPosition(const glm::vec3 &vec);