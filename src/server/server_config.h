#pragma once

#include <common/types.h>
#include <common/net_constants.h>

namespace server
{
    struct Config 
    {
        port_t port = DEFAULT_PORT;
        int maxConnections = 4;
    };
} // namespace server
