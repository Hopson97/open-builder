#pragma once

#include <unordered_map>
#include <SFML/System/Vector3.hpp>

using ChunkPosition = sf::Vector3i;
using BlockPosition = sf::Vector3i;

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