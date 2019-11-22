#pragma once

namespace server {
    struct Config {
        bool fullScreen = false;
        int windowWidth = 1280;
        int windowHeight = 720;
        int fpsLimit = 60;
        int fov = 65;

        bool isFpsCapped = true;
    };

} // namespace server
