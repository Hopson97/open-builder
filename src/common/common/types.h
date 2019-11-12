#pragma once

#include <SFML/System/Vector3.hpp>
#include <cstdint>
#include <unordered_map>

// Integral types
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;

// Network types
using Port = u16;
using ClientId = u8;
using EntityId = u16;
using Command = u8;
using Input = u8;

// Game types
using BlockPosition = sf::Vector3<i32>;
using ChunkPosition = sf::Vector3<i32>;

struct ChunkPositionHash {
    // http://www.beosil.com/download/CollisionDetectionHashing_VMV03.pdf
    std::size_t operator()(const ChunkPosition &position) const
    {
        return (position.x * 130199) ^ (position.y * 146437) ^
               (position.z * 178571);
    }
};

template <typename T>
using ChunkPositionMap =
    std::unordered_map<ChunkPosition, T, ChunkPositionHash>;