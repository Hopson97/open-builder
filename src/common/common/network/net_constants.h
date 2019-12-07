#pragma once

#include "net_types.h"

// The default port the server socket is bounds to (And what clients send data to)
constexpr port_t DEFAULT_PORT = 52666;

// The minimum number connections the server can hold
constexpr int MIN_CONNECTIONS = 1;

// The maximum number of connections the server can hold simultaneously
constexpr int MAX_CONNECTIONS = 8;