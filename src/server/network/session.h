#pragma once

#include <common/network/endpoint.h>


namespace server {
    struct Entity;

    struct ClientSession {
        Endpoint endpoint;
        input_t keyState = 0;
        Entity *p_entity = nullptr;
    };

    enum class ClientStatus : u8 {
        Connected,
        Connecting,
        Disconnected,
    };
} // namespace server
