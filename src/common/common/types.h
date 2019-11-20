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
using port_t = u16;
using peer_id_t = u8;
using entityid_t = u16;
using command_t = u8;
using input_t = u8;

// Game types
using BlockPosition = sf::Vector3<i32>;
using ChunkSectionPosition = sf::Vector3<i32>;

struct ChunkPosition {
    ChunkPosition() = default;
    ChunkPosition(int xp, int zp)
        : x(xp)
        , z(zp)
    {
    }

    i32 x = 0;
    i32 z = 0;
};

struct ChunkPositionHash {
    // http://www.beosil.com/download/CollisionDetectionHashing_VMV03.pdf
    std::size_t operator()(const ChunkPosition &position) const
    {
        return (position.x * 130199) ^ (position.z * 146437);
    }
};

template <typename T>
using ChunkPositionMap =
    std::unordered_map<ChunkPosition, T, ChunkPositionHash>;