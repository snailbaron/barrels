#pragma once

#include <string>

enum class WindowMode {
    Windowed,
    Borderless,
    Fullscreen,
};

struct Config {
    int gameFps = 60;
    std::string windowTitle = "Barrels";
    WindowMode windowMode = WindowMode::Windowed;
    int windowWidth = 1024;
    int windowHeight = 768;
    int pixelsPerUnit = 16;
    int pixelZoom = 8;
};

const Config& config();
