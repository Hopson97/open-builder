#pragma once

struct LaunchConfig {
    struct GraphicsOptions {
        bool fullScreen = false;
        int windowWidth = 1280;
        int windowHeight = 720;
        int fpsLimit = 60;
        int fov = 65;

        bool isFpsCapped = true;
    } graphicOptions;

    enum class Type {
        Server,
        Client,
        Both,
    } type = Type::Both;
};