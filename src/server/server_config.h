#pragma once

#include <common/types.h>

namespace server
{
    struct Config 
    {
        port_t port;
        int maxConnections;
    };
} // namespace server
