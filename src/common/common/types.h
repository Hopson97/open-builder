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