#pragma once

#include "events.hpp"

#include <thing.hpp>
#include <ve.hpp>

#include <cstdint>
#include <vector>

namespace cmp {

struct Movement {
    Vector position;
    Vector velocity;
    Vector acceleration;
    float maxSpeed = 10.f;
    float accelerationTime = 1.f;
    float decelerationTime = 0.5f;
};

struct Control {
    Vector control;
};

} // namespace

class World : public evening::Subscriber {
public:
    World();

    void init();

    void update(double delta);

private:
    thing::EntityManager _ecs;
};
