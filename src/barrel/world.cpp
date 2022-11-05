#include "world.hpp"

#include <algorithm>
#include <iostream>

World::World()
{
    subscribe<evt::Control>(events, [this] (const evt::Control& e) {
        _ecs.component<cmp::Control>(e.entity).control = unit(e.control);
        std::cerr << "setting control for " << e.entity << ": " <<
            unit(e.control) << "\n";
    });
}

void World::init()
{
    auto hero = _ecs.createEntity();
    {
        _ecs.add(hero, cmp::Movement{});
        _ecs.add(hero, cmp::Control{});
        events.push(evt::EntitySpawned{
            .entity = hero, .type = EntityType::Hero});
    }
}

void World::update(double delta)
{
    auto acceleration = [] (const cmp::Movement& movement) {
        return movement.maxSpeed / movement.accelerationTime;
    };
    auto deceleration = [] (const cmp::Movement& movement) {
        return movement.maxSpeed / movement.decelerationTime;
    };

    for (const auto e : _ecs.entities<cmp::Control>()) {
        const auto& control = _ecs.component<cmp::Control>(e).control;
        auto& movement = _ecs.component<cmp::Movement>(e);
        movement.velocity += control *
            (acceleration(movement) + deceleration(movement)) * delta;
    }

    for (const auto e : _ecs.entities<cmp::Movement>()) {
        auto& movement = _ecs.component<cmp::Movement>(e);

        const auto deceleration = movement.maxSpeed / movement.decelerationTime;
        auto speed = length(movement.velocity);
        if (speed > 0) {
            auto targetSpeed = std::clamp(
                static_cast<float>(speed - deceleration * delta),
                0.f,
                movement.maxSpeed);
            movement.velocity *= targetSpeed / speed;
            speed = targetSpeed;
        }

        if (speed > 0) {
            movement.position += movement.velocity * delta;
            events.push(evt::EntityMoved{
                .entity = e, .position = movement.position});
        }
    }
}
