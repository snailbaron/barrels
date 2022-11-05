#pragma once

#include <evening.hpp>
#include <thing.hpp>
#include <ve.hpp>

template <class T> struct XYModel {
    T x;
    T y;
};
using Vector = ve::Vector<XYModel, float>;

inline evening::Channel events;

enum class EntityType {
    Unknown,
    Hero,
};

namespace evt {

struct Frame {
    size_t frameIndex = 0;
};

struct Control {
    thing::Entity entity;
    Vector control;
};

struct EntitySpawned {
    thing::Entity entity;
    Vector position;
    EntityType type = EntityType::Unknown;
};

struct EntityMoved {
    thing::Entity entity;
    Vector position;
};

} // namespace evt
