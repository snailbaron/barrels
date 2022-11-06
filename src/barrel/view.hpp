#pragma once

#include "events.hpp"

#include <thing.hpp>

#include <SDL.h>

#include <map>
#include <optional>

struct KeyboardControlState {
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
};

class Camera {
public:
    [[nodiscard]] Vector project(const Vector& worldPosition) const;

    void lookAt(const Vector& worldCenter);
    void viewport(int width, int height);

private:
    Vector _worldCenter;
    Vector _viewportSize;
};

class View : public evening::Subscriber {
public:
    View();

    bool processInput();
    void update(double delta);
    void present() const;

private:
    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
    Camera _camera;
    KeyboardControlState _keyboardControlState;
    std::optional<thing::Entity> _controlledEntity;

    std::map<thing::Entity, Vector> _positions;
};
