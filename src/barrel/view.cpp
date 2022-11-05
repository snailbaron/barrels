#include "view.hpp"

#include "config.hpp"
#include "events.hpp"

#include <iostream>
#include <utility>

Vector Camera::project(const Vector& worldPosition) const
{
    const auto ratio = config().pixelsPerUnit * config().pixelZoom;
    const auto screenCenter = _viewportSize / 2;

    return {
        screenCenter.x + (worldPosition.x - _worldCenter.x) * ratio,
        screenCenter.y - (worldPosition.y - _worldCenter.y) * ratio
    };
}

void Camera::lookAt(Vector worldCenter)
{
    _worldCenter = std::move(worldCenter);
}

void Camera::viewport(Vector viewportSize)
{
    _viewportSize = std::move(viewportSize);
}

View::View()
{
    subscribe<evt::EntitySpawned>(events, [this] (const evt::EntitySpawned& e) {
        _positions.emplace(e.entity, e.position);
        if (e.type == EntityType::Hero) {
            _controlledEntity = e.entity;
        }
    });
    subscribe<evt::EntityMoved>(events, [this] (const evt::EntityMoved& e) {
        _positions.at(e.entity) = e.position;
    });

    Uint32 flags = 0;
    if (config().windowMode == WindowMode::Borderless) {
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    } else if (config().windowMode == WindowMode::Fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    _window = sdlCheck(SDL_CreateWindow(
        config().windowTitle.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        config().windowWidth,
        config().windowHeight,
        flags));

    {
        int width = 0;
        int height = 0;
        SDL_GetWindowSize(_window, &width, &height);
        _camera.viewport({1.f * width, 1.f * height});
    }

    _renderer = sdlCheck(SDL_CreateRenderer(
        _window,
        -1,
        SDL_RENDERER_ACCELERATED));
}

bool View::processInput()
{
    bool controlChanged = false;
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return false;
        } else if ((e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) &&
                e.key.repeat) {
            continue;
        } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q) {
            return false;
        } else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
            if (e.key.keysym.sym == SDLK_a) {
                _keyboardControlState.left = (e.type == SDL_KEYDOWN);
                controlChanged = true;
            } else if (e.key.keysym.sym == SDLK_d) {
                _keyboardControlState.right = (e.type == SDL_KEYDOWN);
                controlChanged = true;
            } else if (e.key.keysym.sym == SDLK_w) {
                _keyboardControlState.up = (e.type == SDL_KEYDOWN);
                controlChanged = true;
            } else if (e.key.keysym.sym == SDLK_s) {
                _keyboardControlState.down = (e.type == SDL_KEYDOWN);
                controlChanged = true;
            }
        }
    }

    if (_controlledEntity && controlChanged) {
        events.push(evt::Control{
            .entity = *_controlledEntity,
            .control = {
                0.f + _keyboardControlState.right - _keyboardControlState.left,
                0.f + _keyboardControlState.up - _keyboardControlState.down,
            },
        });
    }

    return true;
}

void View::update(double delta)
{
}

void View::present() const
{
    sdlCheck(SDL_SetRenderDrawColor(_renderer, 50, 50, 50, 255));
    sdlCheck(SDL_RenderClear(_renderer));

    for (const auto& [e, position] : _positions) {
        const auto p = _camera.project(position);
        const auto offset = config().pixelsPerUnit * config().pixelZoom / 2;
        const auto color = SDL_Color{200, 200, 200, 255};

        auto vs = std::vector<SDL_Vertex> {
            SDL_Vertex{.position = {p.x, p.y - offset}, .color = color},
            SDL_Vertex{.position = {p.x - offset, p.y}, .color = color},
            SDL_Vertex{.position = {p.x, p.y + offset}, .color = color},
            SDL_Vertex{.position = {p.x + offset, p.y}, .color = color},
        };
        auto indices = std::vector<int> {0, 1, 2, 0, 2, 3};

        sdlCheck(SDL_RenderGeometry(
            _renderer, nullptr, vs.data(), vs.size(), indices.data(), indices.size()));
    }

    SDL_RenderPresent(_renderer);
}
