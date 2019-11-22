#pragma once

#include <common/net_constants.h>
#include <common/types.h>

namespace server {
    struct Config {
        port_t port = DEFAULT_PORT;
        int maxConnections = 4;
    };
} // namespace server
